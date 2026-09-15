/* qlibc - A light-weight and portable C standard library
 * Copyright (C) 2025 Qiu Yixiang
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

// Constants for strerror for i386

ERROR(0, "Success"), ERROR(EPERM, "Operation not permitted"),
    ERROR(ENOENT, "No such file or directory"), ERROR(ESRCH, "No such process"),
    ERROR(EINTR, "Interrupted system call"), ERROR(EIO, "Input/output error"),
    ERROR(ENXIO, "No such device or address"),
    ERROR(E2BIG, "Argument list too long"), ERROR(ENOEXEC, "Exec format error"),
    ERROR(EBADF, "Bad file descriptor"), ERROR(ECHILD, "No child processes"),
    ERROR(EAGAIN, "Resource temporarily unavailable"),
    ERROR(ENOMEM, "Cannot allocate memory"), ERROR(EACCES, "Permission denied"),
    ERROR(EFAULT, "Bad address"), ERROR(ENOTBLK, "Block device required"),
    ERROR(EBUSY, "Device or resource busy"), ERROR(EEXIST, "File exists"),
    ERROR(EXDEV, "Invalid cross-device link"), ERROR(ENODEV, "No such device"),
    ERROR(ENOTDIR, "Not a directory"), ERROR(EISDIR, "Is a directory"),
    ERROR(EINVAL, "Invalid argument"),
    ERROR(ENFILE, "Too many open files in system"),
    ERROR(EMFILE, "Too many open files"),
    ERROR(ENOTTY, "Inappropriate ioctl for device"),
    ERROR(ETXTBSY, "Text file busy"), ERROR(EFBIG, "File too large"),
    ERROR(ENOSPC, "No space left on device"), ERROR(ESPIPE, "Illegal seek"),
    ERROR(EROFS, "Read-only file system"), ERROR(EMLINK, "Too many links"),
    ERROR(EPIPE, "Broken pipe"),
    ERROR(EDOM, "Numerical argument out of domain"),
    ERROR(ERANGE, "Numerical result out of range"),
    ERROR(EDEADLK, "Resource deadlock avoided"),
    ERROR(ENAMETOOLONG, "File name too long"),
    ERROR(ENOLCK, "No locks available"),
    ERROR(ENOSYS, "Function not implemented"),
    ERROR(ENOTEMPTY, "Directory not empty"),
    ERROR(ELOOP, "Too many levels of symbolic links"),
    ERROR(EWOULDBLOCK, "Operation would block"),
    ERROR(ENOMSG, "No message of desired type"),
    ERROR(EIDRM, "Identifier removed"),
    ERROR(ECHRNG, "Channel number out of range"),
    ERROR(EL2NSYNC, "Level 2 not synchronized"),
    ERROR(EL3HLT, "Level 3 halted"), ERROR(EL3RST, "Level 3 reset"),
    ERROR(ELNRNG, "Link number out of range"),
    ERROR(EUNATCH, "Protocol driver not attached"),
    ERROR(ENOCSI, "No CSI structure available"),
    ERROR(EL2HLT, "Level 2 halted"), ERROR(EBADE, "Invalid exchange"),
    ERROR(EBADR, "Invalid request descriptor"), ERROR(EXFULL, "Exchange full"),
    ERROR(ENOANO, "No anode"), ERROR(EBADRQC, "Invalid request code"),
    ERROR(EBADSLT, "Invalid slot"),
    ERROR(EILSEQ, "Invalid or incomplete multibyte or wide character")