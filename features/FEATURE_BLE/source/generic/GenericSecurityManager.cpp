/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 ARM Limited
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

#ifndef __GENERIC_SECURITY_MANAGER_H__
#define __GENERIC_SECURITY_MANAGER_H__

#include "SecurityManager.h"
#include "PalSecurityManager.h"
#include "Callback.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"

namespace ble {
namespace generic {

using ble::pal::address_t;
using ble::pal::advertising_peer_address_type_t;
using ble::pal::irk_t;
using ble::pal::csrk_t;
using ble::pal::ltk_t;
using ble::pal::ediv_t;
using ble::pal::rand_t;
using ble::pal::AuthenticationMask;
using ble::pal::KeyDistribution;
using ble::pairing_failure_t;
using ble::pal::PasskeyAsci;
using ble::pal::passkey_num_t;
typedef SecurityManager::SecurityIOCapabilities_t SecurityIOCapabilities_t;

/* separate structs to allow db implementation to minimise memory usage */

struct SecurityEntry_t {
    connection_handle_t handle;
    address_t peer_identity_address;
    uint8_t encryption_key_size;
    uint8_t peer_address_public:1;
    uint8_t mitm_performed:1;
    uint8_t mitm:1; /**< does the key provide mitm */
    uint8_t connected:1;
    uint8_t authenticated:1; /**< have we authenticated during this connection */
    uint8_t sign_data:1;
    uint8_t encryption_requested:1;
    uint8_t encrypted:1;
    uint8_t oob:1;
    uint8_t oob_mitm_protection:1;
    uint8_t secure_connections:1;
    uint8_t signing_key:1;
    uint8_t encryption_key:1;
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

class GenericSecurityManagerEventHandler;

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
     * @param[in] handle valid connection handle
     * @return pointer to security entry, NULL if handle was invalid
     */
    virtual SecurityEntry_t* get_entry(connection_handle_t connection);


    virtual void get_entry_csrk(
        SecurityEntryCsrkDbCb_t cb,
        connection_handle_t connection
    );
    virtual void get_entry_keys(
        SecurityEntryKeysDbCb_t cb,
        const ediv_t ediv,
        const rand_t rand
    );
    virtual void get_entry_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection
    );
    virtual void get_entry_identityt(
        SecurityEntryIdentityDbCb_t cb,
        address_t &identity_address
    );

    virtual void update_entry(
        connection_handle_t connection,
        bool address_is_public,
        const address_t &peer_address,
        const ediv_t ediv,
        const rand_t rand,
        const ltk_t ltk,
        const irk_t irk,
        const csrk_t csrk
    );
    virtual void update_entry_ltk(
        connection_handle_t connection,
        const ltk_t ltk
    );
    virtual void update_entry_ediv_rand(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    );
    virtual void update_entry_irk(
        connection_handle_t connection,
        const irk_t irk
    );
    virtual void update_entry_bdaddr(
        connection_handle_t connection,
        bool address_is_public,
        const address_t &peer_address
    );
    virtual void update_entry_csrk(
        connection_handle_t connection,
        const csrk_t csrk
    );

    virtual void remove_entry(SecurityEntry_t&);
    virtual void clear_entries();

    virtual void get_whitelist(WhitelistDbCb_t cb);

    virtual void update_whitelist(Gap::Whitelist_t& whitelist);
    virtual void add_whitelist_entry(const address_t &address);

    virtual void remove_whitelist_entry(const address_t &address);
    virtual void clear_whitelist();

    virtual void restore();
    virtual void sync();
    virtual void set_restore(bool reload);
};

class GenericSecurityManager : public SecurityManager,
                               public ble::pal::SecurityManagerEventHandler {
public:
    ////////////////////////////////////////////////////////////////////////////
    // SM lifecycle management
    //
    virtual ble_error_t init(
        bool bondable = true,
        bool mitm     = true,
        SecurityIOCapabilities_t iocaps = IO_CAPS_NONE,
        const Passkey_t passkey  = NULL,
        bool signing = true
    ) {
        db.restore();
        pal.set_io_capability((io_capability_t::type) iocaps);
        pal.set_display_passkey(PasskeyAsci::to_num(passkey));
        legacy_pairing_allowed = true;

        bool secure_connections;
        pal.get_secure_connections_support(secure_connections);

        authentication.set_bondable(bondable);
        authentication.set_mitm(mitm);
        authentication.set_secure_connections(secure_connections);
        authentication.set_keypress_notification(true);

        key_distribution.set_signing(signing);

        return BLE_ERROR_NONE;
    }

    virtual ble_error_t reset(void) {
        db.sync();
        SecurityManager::reset();

        return BLE_ERROR_NONE;
    }

    virtual ble_error_t preserveBondingStateOnReset(bool enabled) {
        db.set_restore(enabled);
        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // List management
    //

    virtual ble_error_t purgeAllBondingState(void) {
        db.clear_entries();
        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    virtual ble_error_t requestPairing(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            return pal.send_pairing_request(
                connection,
                entry->oob,
                authentication,
                key_distribution,
                key_distribution
            );
        } else {
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    virtual ble_error_t acceptPairingRequest(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            return pal.send_pairing_response(
                connection,
                entry->oob,
                authentication,
                key_distribution,
                key_distribution
            );
        } else {
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    virtual ble_error_t canceltPairingRequest(connection_handle_t connection) {
        return pal.cancel_pairing(connection, pairing_failure_t::UNSPECIFIED_REASON);
    }

    virtual ble_error_t setPairingRequestAuthorisation(bool required = true) {
        pairing_authorisation_required = required;
        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    virtual ble_error_t allowLegacyPairing(bool allow = true) {
        legacy_pairing_allowed = allow;
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t getSecureConnectionsSupport(bool *enabled) {
        return pal.get_secure_connections_support(*enabled);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    virtual ble_error_t setIoCapability(SecurityIOCapabilities_t iocaps) {
        return pal.set_io_capability((io_capability_t::type) iocaps);
    }

    virtual ble_error_t setDisplayPasskey(const Passkey_t passkey) {
        return pal.set_display_passkey(PasskeyAsci::to_num(passkey));
    }

    virtual ble_error_t setAuthenticationTimeout(
        connection_handle_t connection,
        uint32_t timeout_in_ms
    ) {
        return pal.set_authentication_timeout(connection, timeout_in_ms / 10);
    }

    virtual ble_error_t getAuthenticationTimeout(
        connection_handle_t connection,
        uint32_t *timeout_in_ms
    ) {
        uint16_t timeout_in_10ms;
        ble_error_t status = pal.get_authentication_timeout(connection, timeout_in_10ms);
        *timeout_in_ms = 10 * timeout_in_10ms;
        return status;
    }

    virtual ble_error_t setLinkSecurity(
        connection_handle_t connection,
        SecurityMode_t securityMode
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }
        if (entry->encryption_requested) {
            return BLE_ERROR_OPERATION_NOT_PERMITTED;
        }

        switch (securityMode) {
            case SECURITY_MODE_ENCRYPTION_OPEN_LINK:
                return setLinkEncryption(connection, link_encryption_t::NOT_ENCRYPTED);
                break;

            case SECURITY_MODE_ENCRYPTION_NO_MITM:
                return setLinkEncryption(connection, link_encryption_t::ENCRYPTED);
                break;

            case SECURITY_MODE_ENCRYPTION_WITH_MITM:
                return setLinkEncryption(connection, link_encryption_t::ENCRYPTED_WITH_MITM);
                break;

            case SECURITY_MODE_SIGNED_NO_MITM:
                return getSigningKey(connection, false);
                break;

            case SECURITY_MODE_SIGNED_WITH_MITM:
                return getSigningKey(connection, true);
                break;

            default:
                return BLE_ERROR_INVALID_PARAM;
                break;
        }
    }

    virtual ble_error_t setKeypressNotification(bool enabled = true) {
        authentication.set_keypress_notification(enabled);
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t enableSigning(
        connection_handle_t connection,
        bool enabled = true
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }
        if (!entry->signing_key && enabled) {
            KeyDistribution distribution = key_distribution;
            distribution.set_signing(enabled);
            return pal.send_pairing_request(
                connection,
                entry->oob,
                authentication,
                distribution,
                distribution
            );
        }
        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /**
     * Get the security status of a connection.
     *
     * @param[in]  connection   Handle to identify the connection.
     * @param[out] securityStatusP    Security status.
     *
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t getLinkEncryption(
        connection_handle_t connection,
        link_encryption_t *encryption
    ) {

        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }

        if (entry->encrypted) {
            if (entry->mitm) {
                *encryption = link_encryption_t::ENCRYPTED_WITH_MITM;
            } else {
                *encryption = link_encryption_t::ENCRYPTED;
            }
        } else if (entry->encryption_requested) {
            *encryption = link_encryption_t::ENCRYPTION_IN_PROGRESS;
        } else {
            *encryption = link_encryption_t::NOT_ENCRYPTED;
        }

        return BLE_ERROR_NONE;
    }

    virtual ble_error_t setLinkEncryption(
        connection_handle_t connection,
        link_encryption_t encryption
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }

        link_encryption_t current_encryption(link_encryption_t::NOT_ENCRYPTED);
        getLinkEncryption(connection, &current_encryption);

        if (current_encryption == link_encryption_t::ENCRYPTION_IN_PROGRESS) {
            return BLE_ERROR_OPERATION_NOT_PERMITTED;
        }

        /* ignore if the link is already at required state*/
        if (current_encryption == encryption) {
            return BLE_ERROR_NONE;
        }

        switch(encryption.value()) {
            case link_encryption_t::NOT_ENCRYPTED:
                if (entry->encrypted) {
                    return pal.disable_encryption(connection);
                }
                break;

            case link_encryption_t::ENCRYPTED:
                /* if already better than encrypted don't bother */
                if (current_encryption == link_encryption_t::ENCRYPTED_WITH_MITM) {
                    return BLE_ERROR_NONE;
                }
                entry->encryption_requested = true;
                return enable_encryption(connection);
                break;

            case link_encryption_t::ENCRYPTED_WITH_MITM:
                if (entry->mitm && !entry->encrypted) {
                    entry->encryption_requested = true;
                    return enable_encryption(connection);
                } else {
                    entry->encryption_requested = true;
                    return requestAuthentication(connection);
                }
                break;

            default:
                return BLE_ERROR_INVALID_PARAM;
        }

        return BLE_ERROR_NONE;
    }

    virtual ble_error_t getEncryptionKeySize(
        connection_handle_t connection,
        uint8_t *size
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            *size = entry->encryption_key_size;
            return BLE_ERROR_NONE;
        } else {
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    virtual ble_error_t setEncryptionKeyRequirements(
        uint8_t minimumByteSize,
        uint8_t maximumByteSize
    ) {
        return pal.set_encryption_key_requirements(minimumByteSize, maximumByteSize);
    }

    virtual ble_error_t enable_encryption(connection_handle_t connection) {
        db.get_entry_keys(
            mbed::callback(this, &GenericSecurityManager::enable_encryption_cb),
            connection
        );
        return BLE_ERROR_NONE;
    }

    /**
     * Returns the requested LTK to the PAL. Called by the security db.
     *
     * @param entry security entry returned by the database.
     * @param entryKeys security entry containing keys.
     *
     * @return no action instruction to the db since this only reads the keys.
     */
    DbCbAction_t enable_encryption_cb(
        SecurityEntry_t& entry,
        SecurityEntryKeys_t& entryKeys
    ) {
        pal.enable_encryption(entry.handle, entryKeys.ltk, entryKeys.ediv, entryKeys.rand);
        return DB_CB_ACTION_NO_UPDATE_REQUIRED;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Privacy
    //

    virtual ble_error_t setPrivateAddressTimeout(uint16_t timeout_in_seconds) {
       return pal.set_private_address_timeout(timeout_in_seconds);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    virtual ble_error_t getSigningKey(connection_handle_t connection, bool authenticated) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }
        if (entry->signing_key && (entry->mitm || !authenticated)) {
            /* we have a key that is either authenticated or we don't care if it is
             * so retrieve it from the db now */
            db.get_entry_csrk(
                mbed::callback(this, &GenericSecurityManager::return_csrk_cb),
                connection
            );
            return BLE_ERROR_NONE;
        } else {
            /* we don't have the right key so we need to get it first
             * keys exchange will create the signingKey event */
            if (authenticated) {
                return requestAuthentication(connection);
            } else {
                return requestPairing(connection);
            }
        }
    }

    /**
     * Returns the requested LTK to the PAL. Called by the security db.
     *
     * @param entry security entry returned by the database.
     * @param entryKeys security entry containing keys.
     *
     * @return no action instruction to the db since this only reads the keys.
     */
    DbCbAction_t set_ltk_cb(
        SecurityEntry_t& entry,
        SecurityEntryKeys_t& entryKeys
    ) {
        pal.set_ltk(entry.handle, entryKeys.ltk);
        return DB_CB_ACTION_NO_UPDATE_REQUIRED;
    }

    DbCbAction_t return_csrk_cb(
        connection_handle_t connection,
        csrk_t csrk
    ) {
        _app_event_handler->signingKey(
            connection,
            csrk,
            db.get_entry(connection)->mitm
        );
        return DB_CB_ACTION_NO_UPDATE_REQUIRED;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    virtual ble_error_t requestAuthentication(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }

        if (entry->mitm) {
            if (entry->authenticated) {
                return BLE_ERROR_NONE;
            } else {
                entry->encryption_requested = true;
                return enable_encryption(connection);
            }
        } else {
            /* don't change the default value of authentication */
            AuthenticationMask connection_authentication = authentication;
            connection_authentication.set_mitm(true);
            return pal.send_pairing_request(
                connection,
                entry->oob,
                authentication,
                key_distribution,
                key_distribution
            );
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    virtual ble_error_t setOOBDataUsage(
        connection_handle_t connection,
        bool useOOB,
        bool OOBProvidesMITM = true
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            entry->oob = useOOB;
            entry->oob_mitm_protection = OOBProvidesMITM;
            return BLE_ERROR_NONE;
        } else {
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    virtual ble_error_t confirmationEntered(
        connection_handle_t connection,
        bool confirmation
    ) {
        return pal.confirmation_entered(connection, confirmation);
    }

    virtual ble_error_t passkeyEntered(
        connection_handle_t connection,
        Passkey_t passkey
    ) {
        return pal.passkey_request_reply(
            connection,
            PasskeyAsci::to_num(passkey)
        );
    }

    virtual ble_error_t sendKeypressNotification(
        connection_handle_t connection,
        Keypress_t keypress
    ) {
        return pal.send_keypress_notification(connection, keypress);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Event handler
    //

    virtual void setSecurityManagerEventHandler(
        ::SecurityManager::SecurityManagerEventHandler* handler
     ) {
        SecurityManager::setSecurityManagerEventHandler(handler);
        if (handler) {
            _app_event_handler = handler;
        }
    }

protected:
    GenericSecurityManager(ble::pal::SecurityManager& palImpl)
        : pal(palImpl),
          pairing_authorisation_required(false),
          legacy_pairing_allowed(true),
          authentication(0),
          key_distribution(KeyDistribution::KEY_DISTRIBUTION_ALL) {
        _app_event_handler = &defaultEventHandler;
        pal.set_event_handler(this);
    }

private:
    ble::pal::SecurityManager& pal;
    SecurityDb db;

    bool pairing_authorisation_required;
    bool legacy_pairing_allowed;

    AuthenticationMask  authentication;
    KeyDistribution     key_distribution;

    /*  implements ble::pal::SecurityManagerEventHandler */
public:

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    virtual void on_pairing_request(
        connection_handle_t connection,
        bool use_oob,
        AuthenticationMask authentication,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    ) {
        set_mitm_performed(connection, false);

        if (pairing_authorisation_required) {
            _app_event_handler->acceptPairingRequest(connection);
        }
    }

    virtual void on_pairing_error(
        connection_handle_t connection,
        pairing_failure_t error
    ) {
        set_mitm_performed(connection, false);

        _app_event_handler->pairingResult(
            connection,
            (SecurityManager::SecurityCompletionStatus_t)(error.value() | 0x80)
        );
    }

    virtual void on_pairing_timed_out(connection_handle_t connection) {
        set_mitm_performed(connection, false);

        _app_event_handler->pairingResult(
            connection,
            SecurityManager::SEC_STATUS_TIMEOUT
        );
    }

    virtual void on_pairing_completed(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            if (entry->encryption_requested) {
                enable_encryption(connection);
            }
            entry->mitm = entry->mitm_performed;
            entry->mitm_performed = false;
        }

        _app_event_handler->pairingResult(
            connection,
            SecurityManager::SEC_STATUS_SUCCESS
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    // Security
    //

    virtual void on_valid_mic_timeout(connection_handle_t connection) {
        _app_event_handler->validMicTimeout(connection);
    }

    virtual void on_slave_security_request(
        connection_handle_t connection,
        AuthenticationMask authentication
    ) {

    }

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    virtual void on_link_encryption_result(
        connection_handle_t connection,
        link_encryption_t result
    ) {
        if (result == link_encryption_t::ENCRYPTED
            || result == link_encryption_t::ENCRYPTED_WITH_MITM) {
            SecurityEntry_t *entry = db.get_entry(connection);
            if (entry) {
               entry->encryption_requested = false;
            }
        }
        _app_event_handler->linkEncryptionResult(connection, result);
    }

    virtual void on_link_encryption_request_timed_out(
        connection_handle_t connection
    ) {
        _app_event_handler->linkEncryptionResult(
            connection,
            link_encryption_t::NOT_ENCRYPTED
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //
    virtual void set_mitm_performed(connection_handle_t connection, bool enable = true) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            entry->mitm_performed = true;
        }
    }

    virtual void on_passkey_display(
        connection_handle_t connection,
        const passkey_num_t passkey
    ) {
        set_mitm_performed(connection);
        _app_event_handler->passkeyDisplay(connection, PasskeyAsci(passkey).asci);
    }

    virtual void on_keypress_notification(
        connection_handle_t connection,
        SecurityManager::Keypress_t keypress
    ) {
        set_mitm_performed(connection);
        _app_event_handler->keypressNotification(connection, keypress);
    }

    virtual void on_passkey_request(connection_handle_t connection) {
        set_mitm_performed(connection);
        _app_event_handler->passkeyRequest(connection);
    }

    virtual void on_confirmation_request(connection_handle_t connection) {
        set_mitm_performed(connection);
        _app_event_handler->confirmationRequest(connection);
    }

    virtual void on_legacy_pairing_oob_request(connection_handle_t connection) {
        set_mitm_performed(connection);
        _app_event_handler->legacyPairingOobRequest(connection);
    }

    virtual void on_oob_request(connection_handle_t connection) {
        set_mitm_performed(connection);
        _app_event_handler->oobRequest(connection);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    virtual void on_keys_distributed(
        connection_handle_t connection,
        advertising_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address,
        const ediv_t ediv,
        const rand_t rand,
        const ltk_t ltk,
        const irk_t irk,
        const csrk_t csrk
    ) {
        db.update_entry(
            connection,
            (peer_address_type == advertising_peer_address_type_t::PUBLIC_ADDRESS),
            peer_identity_address,
            ediv,
            rand,
            ltk,
            irk,
            csrk
        );

        _app_event_handler->signingKey(
            connection,
            csrk,
            db.get_entry(connection)->mitm
        );
    }

    virtual void on_keys_distributed_ltk(
        connection_handle_t connection,
        const ltk_t ltk
    ) {
        db.update_entry_ltk(connection, ltk);
    }

    virtual void on_keys_distributed_ediv_rand(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) {
        db.update_entry_ediv_rand(connection, ediv, rand);
    }

    virtual void on_keys_distributed_irk(
        connection_handle_t connection,
        const irk_t irk
    ) {
        db.update_entry_irk(connection, irk);
    }

    virtual void on_keys_distributed_bdaddr(
        connection_handle_t connection,
        advertising_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address
    ) {
        db.update_entry_bdaddr(
            connection,
            (peer_address_type == advertising_peer_address_type_t::PUBLIC_ADDRESS),
            peer_identity_address
        );
    }

    virtual void on_keys_distributed_csrk(
        connection_handle_t connection,
        const csrk_t csrk
    ) {
        db.update_entry_csrk(connection, csrk);

        _app_event_handler->signingKey(
            connection,
            csrk,
            db.get_entry(connection)->mitm
        );
    }

    virtual void on_ltk_request(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) {
        db.get_entry_keys(
            mbed::callback(this, &GenericSecurityManager::set_ltk_cb),
            ediv,
            rand
        );
    }

private:
    /* handler is always a valid pointer */
    ::SecurityManager::SecurityManagerEventHandler *_app_event_handler;
};


} /* namespace generic */
} /* namespace ble */

#endif /*__GENERIC_SECURITY_MANAGER_H__*/
