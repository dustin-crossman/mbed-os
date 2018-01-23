/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __GENERIC_SECURITY_MANAGER_DB_H__
#define __GENERIC_SECURITY_MANAGER_DB_H__

#include "Callback.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"

namespace ble {
namespace generic {

using ble::pal::address_t;
using ble::pal::irk_t;
using ble::pal::csrk_t;
using ble::pal::ltk_t;
using ble::pal::ediv_t;
using ble::pal::rand_t;

/* separate structs to allow db implementation to minimise memory usage */

struct SecurityEntry_t {
    SecurityEntry_t()
        : handle(0),
        encryption_key_size (0),
        peer_address_public(false),
        csrk_stored(false),
        mitm_csrk(false),
        ltk_stored(false),
        mitm_ltk(false),
        secure_connections(false),
        connected(false),
        authenticated(false),
        master(false),
        encryption_requested(false),
        encrypted(false),
        signing_requested(false),
        mitm_requested(false),
        mitm_performed(false),
        oob(false),
        oob_mitm_protection(false) { }

    void reset() {
        mitm_requested = false;
        mitm_performed = false;
        connected = true;
        authenticated = false;
        encryption_requested = false;
        encrypted = false;
        signing_requested = false;
        oob = false;
        oob_mitm_protection = false;
    }

    connection_handle_t handle;
    address_t peer_identity_address;
    uint8_t encryption_key_size;
    uint8_t peer_address_public:1;

    uint8_t csrk_stored:1;
    uint8_t mitm_csrk:1;
    uint8_t ltk_stored:1;
    uint8_t mitm_ltk:1;
    uint8_t secure_connections:1;

    /* do not store */

    uint8_t connected:1;
    uint8_t authenticated:1; /**< have we turned encryption on during this connection */
    uint8_t master:1;

    uint8_t encryption_requested:1;
    uint8_t encrypted:1;
    uint8_t signing_requested:1;

    uint8_t mitm_requested:1;
    uint8_t mitm_performed:1; /**< keys exchange will have mitm protection */

    uint8_t oob:1;
    uint8_t oob_mitm_protection:1;
};

struct SecurityEntryKeys_t {
    ltk_t  ltk;
    ediv_t ediv;
    rand_t rand;
};

struct SecurityEntryIdentity_t {
    irk_t  irk;
    csrk_t csrk;
};

enum DbCbAction_t {
    DB_CB_ACTION_UPDATE,
    DB_CB_ACTION_NO_UPDATE_REQUIRED, /* does not guarantee discarding changes if you made any */
    DB_CB_ACTION_REMOVE
};

typedef mbed::Callback<DbCbAction_t(SecurityEntry_t&)> SecurityEntryDbCb_t;
typedef mbed::Callback<DbCbAction_t(SecurityEntry_t&, SecurityEntryKeys_t&)> SecurityEntryKeysDbCb_t;
typedef mbed::Callback<DbCbAction_t(connection_handle_t, csrk_t)> SecurityEntryCsrkDbCb_t;
typedef mbed::Callback<DbCbAction_t(SecurityEntry_t&, SecurityEntryIdentity_t&)> SecurityEntryIdentityDbCb_t;
typedef mbed::Callback<DbCbAction_t(Gap::Whitelist_t&)> WhitelistDbCb_t;

/**
 * SecurityDB holds the state for active connections and bonded devices.
 * Keys can be stored in NVM and are returned via callbacks.
 * SecurityDB is responsible for serialising any requests and keeping
 * the store in a consistent state.
 * Active connections state must be returned immediately.
 */
class GenericSecurityDb {
public:
    GenericSecurityDb() { };
    virtual ~GenericSecurityDb() { };

    /**
     * Return immediately security entry containing the state
     * information for active connection.
     * @param[in] handle valid connection handle
     * @return pointer to security entry, NULL if handle was invalid
     */
    virtual SecurityEntry_t* get_entry(
        connection_handle_t connection
    ) = 0;

    /* local keys */

    /* get */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) = 0;

    /* set */
    virtual void set_entry_local_ltk(
        connection_handle_t connection,
        const ltk_t ltk
    ) = 0;

    virtual void set_entry_local_ediv_rand(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) = 0;

    /* peer's keys */

    /* get */
    virtual void get_entry_peer_csrk(
        SecurityEntryCsrkDbCb_t cb,
        connection_handle_t connection
    ) = 0;

    virtual void get_entry_peer_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection
    ) = 0;

    /* set */
    virtual void set_entry_peer(
        connection_handle_t connection,
        bool address_is_public,
        const address_t &peer_address,
        const ediv_t ediv,
        const rand_t rand,
        const ltk_t ltk,
        const irk_t irk,
        const csrk_t csrk
    ) = 0;

    virtual void set_entry_peer_ltk(
        connection_handle_t connection,
        const ltk_t ltk
    ) = 0;

    virtual void set_entry_peer_ediv_rand(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) = 0;

    virtual void set_entry_peer_irk(
        connection_handle_t connection,
        const irk_t irk
    ) = 0;

    virtual void set_entry_peer_bdaddr(
        connection_handle_t connection,
        bool address_is_public,
        const address_t &peer_address
    ) = 0;

    virtual void set_entry_peer_csrk(
        connection_handle_t connection,
        const csrk_t csrk
    ) = 0;

    /* local csrk */

    virtual const csrk_t* get_local_csrk() = 0;
    virtual void set_local_csrk(
        const csrk_t csrk
    ) = 0;

    /* list management */

    /**
     * Create a new entry or retrieve existing stored entry
     * and put it in the live connections store to be retrieved
     * synchronously through connection handle.
     *
     * @param connection this will be the index for live entries.
     * @param peer_address this address will be used to locate existing entry.
     *
     * @return pointer to entry newly created or located existing entry.
     */
    virtual SecurityEntry_t* connect_entry(
        connection_handle_t connection,
        address_t peer_address
    ) = 0;

    /**
     * Create a new entry or retrieve existing stored entry
     * and put it in the live connections store to be retrieved
     * synchronously through connection handle.
     *
     * @param connection this handle will be freed up from the security db
     */
    virtual void disconnect_entry(
        connection_handle_t connection
    ) = 0;

    virtual void remove_entry(
        address_t peer_identity_address
    ) = 0;

    virtual void clear_entries() = 0;

    virtual void get_whitelist(
        WhitelistDbCb_t cb
    ) = 0;

    virtual void update_whitelist(
        Gap::Whitelist_t& whitelist
    ) = 0;

    virtual void add_whitelist_entry(
        const address_t &address
    ) = 0;

    virtual void remove_whitelist_entry(
        const address_t &address
    ) = 0;

    virtual void clear_whitelist() = 0;

    /* saving and loading from nvm */

    virtual void restore() = 0;
    virtual void sync() = 0;
    virtual void set_restore(bool reload) = 0;

protected:
    virtual bool resolve_to_identity_address(
        address_t peer_address,
        address_t identity_address
    ) {
        /*TODO: resolve*/
        return (peer_address == identity_address);
    }
};

/* naive memory implementation for verification */
class MemoryGenericSecurityDb : public GenericSecurityDb {
private:
    struct db_store_t {
        db_store_t() { };
        SecurityEntry_t entry;
        SecurityEntryKeys_t key;
        SecurityEntryIdentity_t identity;
    };
    static const size_t MAX_ENTRIES = 5;

public:
    MemoryGenericSecurityDb() { };
    virtual ~MemoryGenericSecurityDb() { };

    /**
     * Return immediately security entry containing the state
     * information for active connection.
     * @param[in] handle valid connection handle
     * @return pointer to security entry, NULL if handle was invalid
     */
    virtual SecurityEntry_t* get_entry(connection_handle_t connection) {
        SecurityEntry_t *entry = NULL;
        db_store_t *store = get_store(connection);
        if (store) {
            entry = &store->entry;
        }
        return entry;
    }

    /* local keys */

    /* get */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) {
        SecurityEntry_t *entry = NULL;
        db_store_t *store = get_store(connection);
        if (store) {
            entry = &store->entry;
        }
        cb(*entry, _local_keys);
    }

    /* set */
    virtual void set_entry_local_ltk(
        connection_handle_t connection,
        const ltk_t ltk
    ) {
        memcpy(_local_keys.ltk, ltk, sizeof(ltk_t));
    }

    virtual void set_entry_local_ediv_rand(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) {
        memcpy(_local_keys.ediv, ediv, sizeof(ediv_t));
        memcpy(_local_keys.rand, rand, sizeof(rand_t));
    }

    /* peer's keys */

    /* get */
    virtual void get_entry_peer_csrk(
        SecurityEntryCsrkDbCb_t cb,
        connection_handle_t connection
    ) {
        SecurityEntry_t *entry = NULL;
        SecurityEntryIdentity_t *identity = NULL;
        db_store_t *store = get_store(connection);
        if (store) {
            entry = &store->entry;
            identity = &store->identity;
        }
        cb(entry->handle, identity->csrk);
    }

    virtual void get_entry_peer_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection
    ) {
        SecurityEntry_t *entry = NULL;
        SecurityEntryKeys_t *key = NULL;
        db_store_t *store = get_store(connection);
        if (store) {
            entry = &store->entry;
            key = &store->key;
        }
        cb(*entry, *key);
    }

    /* set */
    virtual void set_entry_peer(
        connection_handle_t connection,
        bool address_is_public,
        const address_t &peer_address,
        const ediv_t ediv,
        const rand_t rand,
        const ltk_t ltk,
        const irk_t irk,
        const csrk_t csrk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            store->entry.peer_identity_address = peer_address;
            memcpy(store->key.ltk, ltk, sizeof(ltk_t));
            memcpy(store->key.ediv, ediv, sizeof(ediv_t));
            memcpy(store->key.rand, rand, sizeof(rand_t));
            memcpy(store->identity.irk, irk, sizeof(irk_t));
            memcpy(store->identity.csrk, csrk, sizeof(csrk_t));
        }
    }

    virtual void set_entry_peer_ltk(
        connection_handle_t connection,
        const ltk_t ltk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            memcpy(store->key.ltk, ltk, sizeof(ltk_t));
        }
    }

    virtual void set_entry_peer_ediv_rand(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            memcpy(store->key.ediv, ediv, sizeof(ediv_t));
            memcpy(store->key.rand, rand, sizeof(rand_t));
        }
    }

    virtual void set_entry_peer_irk(
        connection_handle_t connection,
        const irk_t irk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            memcpy(store->identity.irk, irk, sizeof(irk_t));
        }
    }

    virtual void set_entry_peer_bdaddr(
        connection_handle_t connection,
        bool address_is_public,
        const address_t &peer_address
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            store->entry.peer_identity_address = peer_address;
        }
    }

    virtual void set_entry_peer_csrk(
        connection_handle_t connection,
        const csrk_t csrk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            memcpy(store->identity.csrk, csrk, sizeof(csrk_t));
        }
    }

    /* local csrk */

    virtual const csrk_t* get_local_csrk() {
        return &_local_identity.csrk;
    }

    virtual void set_local_csrk(const csrk_t csrk) {
        memcpy(&_local_identity.csrk, csrk, sizeof(csrk_t));
    }

    /* list management */

    /**
     * Create a new entry or retrieve existing stored entry
     * and put it in the live connections store to be retrieved
     * synchronously through connection handle.
     *
     * @param connection this will be the index for live entries.
     * @param peer_address this address will be used to locate existing entry.
     *
     * @return pointer to entry newly created or located existing entry.
     */
    virtual SecurityEntry_t* connect_entry(connection_handle_t connection, address_t peer_address) {
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (_db[i].entry.connected) {
                continue;
            } else if (resolve_to_identity_address(peer_address, _db[i].entry.peer_identity_address)) {
                return &_db[i].entry;
            }
        }

        /* if we din't find one grab the first disconnected slot*/
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (!_db[i].entry.connected) {
                _db[i] = db_store_t();
                return &_db[i].entry;
            }
        }

        return NULL;
    }

    /**
     * Create a new entry or retrieve existing stored entry
     * and put it in the live connections store to be retrieved
     * synchronously through connection handle.
     *
     * @param connection this handle will be freed up from the security db
     */
    virtual void disconnect_entry(connection_handle_t connection) { }

    virtual void remove_entry(address_t peer_identity_address);
    virtual void clear_entries() {
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            _db[i] = db_store_t();
        }
        _local_keys = SecurityEntryKeys_t();
        _local_identity = SecurityEntryIdentity_t();
    }

    virtual void get_whitelist(WhitelistDbCb_t cb) { }

    virtual void update_whitelist(Gap::Whitelist_t& whitelist) { }
    virtual void add_whitelist_entry(const address_t &address) { }

    virtual void remove_whitelist_entry(const address_t &address) { }
    virtual void clear_whitelist() { }

    /* saving and loading from nvm */

    virtual void restore() { }
    virtual void sync() { }
    virtual void set_restore(bool reload) { }

private:

    virtual db_store_t* get_store(connection_handle_t connection) {
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (!_db[i].entry.connected) {
                continue;
            } else if (connection == _db[i].entry.handle) {
                return &_db[i];
            }
        }
        return NULL;
    }

    db_store_t _db[MAX_ENTRIES];
    SecurityEntryKeys_t _local_keys;
    SecurityEntryIdentity_t _local_identity;
};

} /* namespace generic */
} /* namespace ble */

#endif /*__GENERIC_SECURITY_MANAGER_DB_H__*/
