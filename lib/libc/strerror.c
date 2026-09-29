/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <kernel/errno.h>
#include <kernel/macros.h>
#include <kernel/string.h>

static const char *const errstrs[] = {
    [0] = "Success",
    [EPERM] = "Operation not permitted",
    [ENOENT] = "No such file or directory",
    [ESRCH] = "No such process",
    [EINTR] = "Interrupted system call",
    [EIO] = "Input/output error",
    [ENXIO] = "No such device or address",
    [E2BIG] = "Argument list too long",
    [ENOEXEC] = "Exec format error",
    [EBADF] = "Bad file descriptor",
    [ECHILD] = "No child processes",
    [EAGAIN] = "Resource temporarily unavailable",
    [ENOMEM] = "Cannot allocate memory",
    [EACCES] = "Permission denied",
    [EFAULT] = "Bad address",
    [ENOTBLK] = "Block device required",
    [EBUSY] = "Device or resource busy",
    [EEXIST] = "File exists",
    [EXDEV] = "Invalid cross-device link",
    [ENODEV] = "No such device",
    [ENOTDIR] = "Not a directory",
    [EISDIR] = "Is a directory",
    [EINVAL] = "Invalid argument",
    [ENFILE] = "Too many open files in system",
    [EMFILE] = "Too many open files",
    [ENOTTY] = "Inappropriate ioctl for device",
    [ETXTBSY] = "Text file busy",
    [EFBIG] = "File too large",
    [ENOSPC] = "No space left on device",
    [ESPIPE] = "Illegal seek",
    [EROFS] = "Read-only file system",
    [EMLINK] = "Too many links",
    [EPIPE] = "Broken pipe",
    [EDOM] = "Numerical argument out of domain",
    [ERANGE] = "Numerical result out of range",
    [EDEADLK] = "Resource deadlock avoided",
    [ENAMETOOLONG] = "File name too long",
    [ENOLCK] = "No locks available",
    [ENOSYS] = "Function not implemented",
    [ENOTEMPTY] = "Directory not empty",
    [ELOOP] = "Too many levels of symbolic links",
    [ENOMSG] = "No message of desired type",
    [EIDRM] = "Identifier removed",
    [ECHRNG] = "Channel number out of range",
    [EL2NSYNC] = "Level 2 not synchronized",
    [EL3HLT] = "Level 3 halted",
    [EL3RST] = "Level 3 reset",
    [ELNRNG] = "Link number out of range",
    [EUNATCH] = "Protocol driver not attached",
    [ENOCSI] = "No CSI structure available",
    [EL2HLT] = "Level 2 halted",
    [EBADE] = "Invalid exchange",
    [EBADR] = "Invalid request descriptor",
    [EXFULL] = "Exchange full",
    [ENOANO] = "No anode",
    [EBADRQC] = "Invalid request code",
    [EBADSLT] = "Invalid slot",
    [EILSEQ] = "Invalid or incomplete multibyte or wide character",
};

char *strerror(int errnum)
{
    if (errnum < 0 || (size_t)errnum >= ARRAY_SIZE(errstrs) || !errstrs[errnum])
        return (char *)"Unknown error";
    return (char *)errstrs[errnum];
}
