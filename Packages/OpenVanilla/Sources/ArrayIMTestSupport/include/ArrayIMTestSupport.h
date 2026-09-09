// Copyright (c) 2022 and onwards The OpenVanilla Authors.
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#ifndef ArrayIMTestSupport_h
#define ArrayIMTestSupport_h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OVArrayLegacySession OVArrayLegacySession;

// Creates a session whose tables are the Array .cin files in
// <dataTablesRoot> (array30.cin, array-shortcode.cin, array-special.cin,
// array-phrase.cin). Returns 0 when the tables cannot be loaded.
OVArrayLegacySession *ov_array_session_create(const char *dataTablesRoot);

// Same as above, but the main table is loaded from <customMainTablePath>
// instead of <dataTablesRoot>/array30.cin (the bundled table remains the
// fallback when the custom table is missing or invalid).
OVArrayLegacySession *ov_array_session_create_with_table(const char *dataTablesRoot, const char *customMainTablePath);

void ov_array_session_destroy(OVArrayLegacySession *session);
void ov_array_session_reset(OVArrayLegacySession *session);

// Sends one printable key; returns the handled flag from keyEvent().
int ov_array_session_key(OVArrayLegacySession *session, char key);

// Same as above, with shift/capslock modifier flags.
int ov_array_session_key_with_modifiers(OVArrayLegacySession *session, char key, int shift, int capslock);

int ov_array_session_is_composing(OVArrayLegacySession *session);
const char *ov_array_session_buffer(OVArrayLegacySession *session);      // composing buffer
const char *ov_array_session_committed(OVArrayLegacySession *session);   // committed text
int ov_array_session_candidates_shown(OVArrayLegacySession *session);
int ov_array_session_candidate_count(OVArrayLegacySession *session);
const char *ov_array_session_candidate_at(OVArrayLegacySession *session, int index);
const char *ov_array_session_last_notify(OVArrayLegacySession *session);
int ov_array_session_beep_count(OVArrayLegacySession *session);

// 特別碼提示 / 快打模式（QuickMode）config helpers.
void ov_array_session_set_auto_sp(OVArrayLegacySession *session, int on);
void ov_array_session_set_force_sp(OVArrayLegacySession *session, int on);
int ov_array_session_is_force_sp(OVArrayLegacySession *session);

#ifdef __cplusplus
}
#endif

#endif /* ArrayIMTestSupport_h */
