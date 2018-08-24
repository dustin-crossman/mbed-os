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

#ifndef NFC_NDEF_MESSAGEBUILDER_H_
#define NFC_NDEF_MESSAGEBUILDER_H_

#include <stdint.h>

#include "platform/Span.h"

#include "nfc/ndef/Record.h"

namespace mbed {
namespace nfc {
namespace ndef {

/**
 * @addtogroup nfc
 * @{
 */

/**
 * Construct a NDEF Message.
 */
class MessageBuilder {

public:
    /**
     * Create a new MessageBuilder that can be used to construct valid NDEF
     * messages.
     *
     * @param buffer The data buffer that will contain the NDEF message.
     */
    MessageBuilder(const Span<uint8_t> &buffer);

    /**
     * Append a new record to the message being built.
     *
     * @param type The type of the record to insert.
     * @param payload The payload of the record (optional).
     * @param is_last_record true if the record to insert is the last record of
     * the payload or false otherwise.
     *
     * @return true if the record has been successfully inserted or false
     * otherwise.
     *
     * @note insertion can fail if the message is already complete or if the
     * size remaining in the message buffer is not large enough to makes the
     * record inserted fit.
     */
    bool append_record(
        const RecordType &type,
        const RecordPayload &payload = RecordPayload(),
        bool is_last_record = false
    );

    /**
     * Append a new record to the message being built.
     *
     * @param record The record to insert.
     *
     * @return true if the record has been successfully inserted or false otherwise.
     *
     * @note insertion can fail if the message is already complete or if the
     * size remaining in the message buffer is not large enough to makes the
     * record inserted fit.
     */
    bool append_record(const Record &record);

    /**
     * Reset the builder state.
     */
    void reset();

    /**
     * Reset the builder state and assign a new buffer to it.
     */
    void reset(const Span<uint8_t> &buffer);

    /**
     * Return true if the message stored is complete and false otherwise.
     *
     * @return true if the message is complete or false.
     */
    bool is_message_complete() const;

    /**
     * Return the buffer storing the data if the message is complete or an empty
     * buffer if the message is not complete.
     *
     * @return The message built.
     */
    Span<const uint8_t> get_message() const;

private:
    // append fields
    void append_header(const Record &record);
    void append_type_length(const Record &record);
    void append_payload_length(const Record &);
    void append_id_length(const Record &);
    void append_type(const Record &);
    void append_id(const Record &);
    void append_payload(const Record &);

    // helpers
    static size_t compute_record_size(const Record &record);
    static bool is_short_payload(const Record &record);

    // builder state.
    Span<uint8_t> _message_buffer;
    size_t _position;
    bool _message_started;
    bool _message_ended;
    bool _in_chunk;
};

/**
 * @}
 */

} // namespace ndef
} // namespace nfc
} // namespace mbed

#endif /* NFC_NDEF_MESSAGEBUILDER_H_ */
