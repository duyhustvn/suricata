/* Copyright (C) 2007-2023 Open Information Security Foundation
 *
 * You can copy, redistribute or modify this Program under the terms of
 * the GNU General Public License version 2 as published by the Free
 * Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/**
 * \file
 *
 * \author Anoop Saldanha <anoopsaldanha@gmail.com>
 */

#ifndef __UTIL_BUFFER_H__
#define __UTIL_BUFFER_H__

typedef struct MemBuffer_ {
    uint32_t size;
    uint32_t offset;
    uint8_t buffer[];
} MemBuffer;

MemBuffer *MemBufferCreateNew(uint32_t size);
int MemBufferExpand(MemBuffer **buffer, uint32_t expand_by);
void MemBufferFree(MemBuffer *buffer);

/**
 * \brief Reset the mem buffer.
 *
 * \param mem_buffer Pointer to the mem buffer instance.
 */
static inline void MemBufferReset(MemBuffer *b)
{
    b->buffer[0] = 0;
    b->offset = 0;
}

/**
 * \brief Get the MemBuffers underlying buffer.
 */
#define MEMBUFFER_BUFFER(mem_buffer) (mem_buffer)->buffer

/**
 * \brief Get the MemBuffers current offset.
 */
#define MEMBUFFER_OFFSET(mem_buffer) (mem_buffer)->offset

/**
 * \brief Get the MemBuffers current size.
 */
#define MEMBUFFER_SIZE(mem_buffer) (mem_buffer)->size

/**
 * \brief Write a buffer to the file pointer.
 *
 *        Accepted buffers can contain both printable and non-printable
 *        characters.  Printable characters are written in the printable
 *        format and the non-printable chars are written in hex codes
 *        using the |XX| format.
 *
 *        For example this would be the kind of output in the file -
 *        onetwo|EF|three|ED|five
 *
 * \param buffer Pointer to the src MemBuffer instance to write.
 * \param fp     Pointer to the file instance to write to.
 */
void MemBufferPrintToFP(MemBuffer *buffer, FILE *fp);

/**
 * \brief Write a buffer to the file pointer as a printable char string.
 *
 * \param b     Pointer to the src MemBuffer instance to write.
 * \param fp    Pointer to the file instance to write to.
 * \retval size_t bytes written by fwrite()
 */
size_t MemBufferPrintToFPAsString(MemBuffer *b, FILE *fp);

/**
 * \brief Write a buffer in hex format.
 *
 * \param b      Pointer to the src MemBuffer instance to write.
 * \param fp     Pointer to the file instance to write to.
 */
void MemBufferPrintToFPAsHex(MemBuffer *b, FILE *fp);

/**
 * \brief Write a raw buffer to the MemBuffer dst.
 *
 *        When we say raw buffer it indicates a buffer that need not be
 *        purely a string buffer.  It can be a pure string buffer or not or
 *        a mixture of both.  Hence we don't accept any format strings.
 *
 *        If the remaining space on the buffer is lesser than the length of
 *        the buffer to write, it is truncated to fit into the empty space.
 *
 *        Also after every write a '\0' is appended.  This would indicate
 *        that the total available space to write in the buffer is
 *        MemBuffer->size - 1 and not Membuffer->size.  The reason we
 *        append the '\0' is for supporting writing pure string buffers
 *        as well, that can later be used by other string handling funcs.
 *
 * \param raw_buffer     The buffer to write.
 * \param raw_buffer_len Length of the above buffer.
 * \retval write_len Bytes written. If less than raw_len, the buffer is full.
 */
uint32_t MemBufferWriteRaw(MemBuffer *dst, const uint8_t *raw, const uint32_t raw_len);

/**
 * \brief Write a string buffer to the Membuffer dst.
 *
 *        This function takes a format string and arguments for the format
 *        string like sprintf.
 *
 *        An example usage of this is -
 *        MemBufferWriteString(mem_buffer_instance, \"%d - %s\", 10, \"one\");
 *
 * \param dst    The dst MemBuffer instance.
 * \param format The format string.
 * \param ...    Variable arguments.
 */
void MemBufferWriteString(MemBuffer *dst, const char *fmt, ...) ATTR_FMT_PRINTF(2, 3);

#endif /* __UTIL_BUFFER_H__ */
