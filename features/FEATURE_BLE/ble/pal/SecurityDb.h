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

#ifndef PAL_SECURITY_MANAGER_DB_H__
#define PAL_SECURITY_MANAGER_DB_H__

#include "platform/Callback.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include <stdlib.h>

namespace ble {
namespace pal {

using pal::connection_peer_address_type_t;

/* separate structs for keys to allow db implementation
 * to minimise memory usage, only holding live connection
 * state in memory */

struct SecurityEntry_t {
    SecurityEntry_t()
        : handle(0),
        encryption_key_size(0),
        peer_address_is_public(false),
        local_address_is_public(false),
        csrk_stored(false),
        csrk_mitm_protected(false),
        ltk_stored(false),
        ltk_mitm_protected(false),
        secure_connections_paired(false),
        connected(false),
        authenticated(false),
        is_master(false),
        encryption_requested(false),
        encryption_failed(false),
        encrypted(false),
        signing_requested(false),
        mitm_requested(false),
        mitm_performed(false),
        attempt_oob(false),
        oob_mitm_protection(false),
        oob_present(false) { }

    /**
     * Reset state of the connection when disconnected.
     */
    void reset() {
        local_address = address_t();

        connected = false;
        authenticated = false;
        is_master = false;

        encryption_requested = false;
        encryption_failed = false;
        encrypted = false;
        signing_requested = false;

        mitm_requested = false;
        mitm_performed = false;

        attempt_oob = false;
        oob_mitm_protection = false;
        oob_present = false;
    }

    connection_handle_t handle;
    address_t peer_address;

    uint8_t encryption_key_size;
    uint8_t peer_address_is_public:1;
    uint8_t local_address_is_public:1;

    uint8_t csrk_stored:1;
    uint8_t csrk_mitm_protected:1;
    uint8_t ltk_stored:1;
    uint8_t ltk_mitm_protected:1;
    uint8_t secure_connections_paired:1;

    /* do not store in NVM */

    address_t local_address; /**< address used for connection, possibly different from identity */

    uint8_t connected:1;
    uint8_t authenticated:1; /**< have we turned encryption on during this connection */
    uint8_t is_master:1;

    uint8_t encryption_requested:1;
    uint8_t encryption_failed:1;
    uint8_t encrypted:1;
    uint8_t signing_requested:1;

    uint8_t mitm_requested:1;
    uint8_t mitm_performed:1; /**< keys exchange will have MITM protection */

    uint8_t attempt_oob:1;
    uint8_t oob_mitm_protection:1;
    uint8_t oob_present:1;
};

struct SecurityEntryKeys_t {
    ltk_t ltk;
    ediv_t ediv;
    rand_t rand;
};

struct SecurityEntryIdentity_t {
    address_t identity_address;
    irk_t irk;
};

/* callbacks for asynchronous data retrieval from the security db */

typedef mbed::Callback<void(const SecurityEntry_t*, const SecurityEntryKeys_t*)> SecurityEntryKeysDbCb_t;
typedef mbed::Callback<void(connection_handle_t, const csrk_t*)> SecurityEntryCsrkDbCb_t;
typedef mbed::Callback<void(Gap::Whitelist_t*)> WhitelistDbCb_t;

/**
 * SecurityDB holds the state for active connections and bonded devices.
 * Keys can be stored in NVM and are returned via callbacks.
 * SecurityDB is responsible for serialising any requests and keeping
 * the store in a consistent state.
 * Active connections state must be returned immediately.
 */
class SecurityDb {
public:
    SecurityDb() { };
    virtual ~SecurityDb() { };

    /**
     * Return immediately security entry containing the state
     * information for active connection.
     *
     * @param[in] handle valid connection handle
     * @return pointer to security entry, NULL if handle was invalid
     */
    virtual SecurityEntry_t* get_entry(
        connection_handle_t connection
    ) = 0;

    /**
     * Return immediately security entry containing the state
     * information for active connection.
     *
     * @param[in] peer_address peer address in the entry being requested
     * @return pointer to security entry, NULL if no entry was fined
     */
    virtual SecurityEntry_t* get_entry(
        const address_t &peer_address
    ) = 0;

    /* local keys */

    /**
     * Retrieve stored LTK based on passed in EDIV and RAND values.
     *
     * @param[in] cb callback that will receive the LTK struct
     * @param[in] connection handle for the connection requesting the key
     * @param[in] ediv one of the values used to identify the LTK
     * @param[in] rand one of the values used to identify the LTK
     */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    ) = 0;

    /**
     * Retrieve stored LTK generated during secure connections pairing.
     *
     * @param[in] cb callback that will receive the LTK struct
     * @param[in] connection handle for the connection requesting the key
     */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection
    ) = 0;

    /**
     * Save new local LTK for a connection.
     *
     * @param[in] connection handle for which the LTK is being updated
     * @param[in] ltk the new LTK, if the device is slave, this is the LTK that will
     *            be used when link is encrypted
     */
    virtual void set_entry_local_ltk(
        connection_handle_t connection,
        const ltk_t &ltk
    ) = 0;

    /**
     * Update EDIV and RAND used to identify the LTK.
     *
     * @param[in] connection handle for the connection which values are being updated
     * @param[in] ediv new EDIV value
     * @param[in] rand new RAND value
     */
    virtual void set_entry_local_ediv_rand(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    ) = 0;

    /* peer's keys */

    /**
     * Return asynchronously the peer signing key through a callback
     * so that signed packets can be verified.
     *
     * @param[in] cb callback which will receive the key
     * @param[in] connection handle of the connection queried
     */
    virtual void get_entry_peer_csrk(
        SecurityEntryCsrkDbCb_t cb,
        connection_handle_t connection
    ) = 0;

    /**
     * Return asynchronously the peer encryption key through a callback
     * so that encryption can be enabled.
     *
     * @param[in] cb callback which will receive the key
     * @param[in] connection handle of the connection queried
     */
    virtual void get_entry_peer_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection
    ) = 0;

    /**
     * Save new LTK received from the peer.
     *
     * @param[in] connection handle for which the LTK is being updated
     * @param[in] ltk the new LTK, if the peer device is slave, this is the LTK that will
     *            be used when link is encrypted
     */
    virtual void set_entry_peer_ltk(
        connection_handle_t connection,
        const ltk_t &ltk
    ) = 0;

    /**
     * Update EDIV and RAND used to identify the LTK sent by the peer.
     *
     * @param[in] connection handle for the connection which values are being updated
     * @param[in] ediv new EDIV value
     * @param[in] rand new RAND value
     */
    virtual void set_entry_peer_ediv_rand(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    ) = 0;

    /**
     * Update IRK for this connection.
     *
     * @param[in] connection handle of the connection being updated
     * @param[in] irk new IRK value
     */
    virtual void set_entry_peer_irk(
        connection_handle_t connection,
        const irk_t &irk
    ) = 0;

    /**
     * Update the identity address of the peer.
     *
     * @param[in] connection connection for the peer address being updated
     * @param[in] address_is_public is the identity address public or private
     * @param[in] peer_address the new address
     */
    virtual void set_entry_peer_bdaddr(
        connection_handle_t connection,
        bool address_is_public,
        const address_t &peer_address
    ) = 0;

    /**
     * Update peer signing key.
     *
     * @param[in] connection handle of the connection being updated
     * @param[in] csrk new CSRK value
     */
    virtual void set_entry_peer_csrk(
        connection_handle_t connection,
        const csrk_t &csrk
    ) = 0;

    /* local csrk */

    /**
     * Return local signing key used for signing packets.
     *
     * @return pointer to local CSRK
     */
    virtual const csrk_t* get_local_csrk() = 0;

    /**
     * Update local signing key.
     *
     * @param[in] csrk new CSRK value
     */
    virtual void set_local_csrk(
        const csrk_t &csrk
    ) = 0;

    /* public keys */

    /**
     * Return local public key.
     *
     * @return ref to x component of public key
     */
    virtual const public_key_t& get_public_key_x() = 0;

    /**
     * Return local public key.
     *
     * @return ref to y component of public key
     */
    virtual const public_key_t& get_public_key_y() = 0;

    /**
     * Set local public key.
     *
     * @param[in] public_key_x new public key value of the x coordinate
     * @param[in] public_key_y new public key value of the y coordinate
     */
    virtual void set_public_key(
        const public_key_t &public_key_x,
        const public_key_t &public_key_y
    ) = 0;

    /* oob data */

    /** There is always only one OOB data set stored at a time */

    /**
     * Return peer address the OOB data belongs to.
     *
     * @return peer address
     */
    virtual const address_t& get_peer_sc_oob_address() = 0;

    /**
     * Return random number from the peer received in OOB data.
     *
     * @return random number the peer chose
     */
    virtual const oob_rand_t& get_peer_sc_oob_random() = 0;

    /**
     * Return confirm number from the peer received in OOB data.
     *
     * @return confirm value calculated by peer based
     * on the random number, its public key and address
     */
    virtual const oob_confirm_t& get_peer_sc_oob_confirm() = 0;

    /**
     * Return OOB data in a single transaction.
     *
     * @param[out] peer_address peer address OOB data belongs to
     * @param[out] peer_random random number the peer chose
     * @param[out] peer_confirm confirm value calculated by peer based
     *                          on the random number, its public key and address
     * @param[out] local_random random number chosen by the local device
     */
    virtual void get_sc_oob_data(
        address_t &peer_address,
        oob_rand_t &peer_random,
        oob_confirm_t &peer_confirm,
        oob_rand_t &local_random
    ) = 0;

    /**
     * Return random number used by the local device to calculate
     * the confirm value sent the peer in OOB data.
     *
     * @return random number chosen by local device
     */
    virtual const oob_rand_t& get_local_sc_oob_random() = 0;

    /**
     * Store the OOB data received from the peer.
     *
     * @param address peer address OOB data belongs to
     * @param random random number the peer chose
     * @param confirm confirm value calculated by peer based
     *                on the random number, its public key and address
     */
    virtual void set_peer_sc_oob_data(
        const address_t &address,
        const oob_rand_t &random,
        const oob_confirm_t &confirm
    ) = 0;

    /**
     * Set random number used for OOB data calculation on the local device.
     *
     * @param random random number chosen by the local device
     */
    virtual void set_local_sc_oob_random(
        const oob_rand_t &random
    ) = 0;

    /* list management */

    /**
     * Create a new entry or retrieve existing stored entry
     * and put it in the live connections store to be retrieved
     * synchronously through connection handle.
     *
     * @param[in] connection this will be the index for live entries.
     * @param[in] peer_address_type type of address
     * @param[in] peer_address this address will be used to locate existing entry.
     *
     * @return pointer to entry newly created or located existing entry.
     */
    virtual SecurityEntry_t* connect_entry(
        connection_handle_t connection,
        BLEProtocol::AddressType_t peer_address_type,
        const address_t &peer_address,
        const address_t &local_address
    ) = 0;

    /**
     * Create a new entry or retrieve existing stored entry
     * and put it in the live connections store to be retrieved
     * synchronously through connection handle.
     *
     * @param[in] connection this handle will be freed up from the security db
     */
    virtual void disconnect_entry(
        connection_handle_t connection
    ) = 0;

    /**
     * Remove entry for this peer from NVM.
     *
     * @param[in] peer_identity_address peer address that no longer needs NVM storage.
     */
    virtual void remove_entry(
        const address_t peer_identity_address
    ) = 0;

    /**
     * Remove all entries from the security DB.
     */
    virtual void clear_entries() = 0;

    /**
     * Asynchronously return the whitelist stored in NVM through a callback. Function
     * takes ownership of the memory. The whitelist and the ownership will be returned
     * in the callback.
     *
     * @param[in] cb callback that will receive the whitelist
     * @param[in] whitelist preallocated whitelist that will be filled in
     */
    virtual void get_whitelist(
        WhitelistDbCb_t cb,
        Gap::Whitelist_t *whitelist
    ) = 0;

    /**
     * Asynchronously return a whitelist through a callback, generated from the bond table.
     * Function takes ownership of the memory. The whitelist and the ownership will be
     * returned in the callback.
     *
     * @param[in] cb callback that will receive the whitelist
     * @param[in] whitelist preallocated whitelist that will be filled in
     */
    virtual void generate_whitelist_from_bond_table(
        WhitelistDbCb_t cb,
        Gap::Whitelist_t *whitelist
    ) = 0;

    /**
     * Update the whitelist stored in NVM by replacing it with new one.
     *
     * @param[in] whitelist
     */
    virtual void set_whitelist(
        const Gap::Whitelist_t &whitelist
    ) = 0;

    /**
     * Add a new entry to the whitelist in the NVM.
     *
     * @param[in] address new whitelist entry
     */
    virtual void add_whitelist_entry(
        const address_t &address
    ) = 0;

    /**
     * Remove whitelist entry from NVM.
     *
     * @param[in] address entry to be removed
     */
    virtual void remove_whitelist_entry(
        const address_t &address
    ) = 0;

    /**
     *Remove all whitelist entries stored in the NVM.
     */
    virtual void clear_whitelist() = 0;

    /* saving and loading from nvm */

    /**
     * Read values from storage.
     */
    virtual void restore() = 0;

    /**
     * Flush all values which might be stored in memory into NVM.
     */
    virtual void sync() = 0;

    /**
     * Toggle whether values should be preserved across resets.
     *
     * @param[in] reload if true values will be preserved across resets.
     */
    virtual void set_restore(bool reload) = 0;
};

} /* namespace pal */
} /* namespace ble */

#endif /*PAL_SECURITY_MANAGER_DB_H__*/
