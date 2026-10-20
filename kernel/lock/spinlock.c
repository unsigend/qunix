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

#include <kernel/compiler.h>
#include <kernel/cpu.h>
#include <kernel/interrupt.h>
#include <kernel/panic.h>
#include <kernel/preempt.h>
#include <kernel/spinlock.h>
#include <kernel/stddef.h>

static void acquire(struct spinlock *lock)
{
#if CONFIG_SMP
    while (atomic_cmpxchg(&lock->locked, 0, 1) != 0) {
        if (lock->owner == cpu_current())
            panic("deadlock detected");
    }
#else
    if (lock->locked)
        panic("deadlock detected");
    lock->locked = true;
#endif /* CONFIG_SMP */

    lock->owner = cpu_current();
}

static bool try_lock(struct spinlock *lock)
{
#if CONFIG_SMP
    if (atomic_cmpxchg(&lock->locked, 0, 1) == 0) {
        lock->owner = cpu_current();
        return true;
    }
#else
    if (!lock->locked) {
        lock->locked = true;
        lock->owner = cpu_current();
        return true;
    }
#endif /* CONFIG_SMP */

    return false;
}

static void release_check(struct spinlock *lock)
{
    if (!spinlock_is_locked(lock))
        panic("unbalanced unlock");

    if (lock->owner != cpu_current())
#if CONFIG_DEBUG && CONFIG_DEBUG_SPINLOCK
        panic(
            "DEBUG: try to unlock a lock that is not owned by the current CPU, "
            "original owned by %s:%s:%d",
            lock->file, lock->func, lock->line);
#else
        panic("lock not owned by current CPU");

#endif /* CONFIG_DEBUG && CONFIG_DEBUG_SPINLOCK */
}

static void release(struct spinlock *lock)
{
    release_check(lock);

    lock->owner = NULL;
#if CONFIG_SMP
    atomic_store(&lock->locked, 0);
#else
    lock->locked = false;
#endif /* CONFIG_SMP */
}

#if CONFIG_DEBUG && CONFIG_DEBUG_SPINLOCK

static void acquire_debug(struct spinlock *lock, const char *file,
                          const char *func, int line)
{
    acquire(lock);
    lock->file = file;
    lock->func = func;
    lock->line = line;
}

static bool try_lock_debug(struct spinlock *lock, const char *file,
                           const char *func, int line)
{
    bool result;
    result = try_lock(lock);
    if (result) {
        lock->file = file;
        lock->func = func;
        lock->line = line;
    }
    return result;
}

bool spinlock_trylock_at(struct spinlock *lock, const char *file,
                         const char *func, int line)
{
    int ret;
    preempt_disable();
    ret = try_lock_debug(lock, file, func, line);
    if (ret)
        return true;
    preempt_enable();
    return false;
}

void spinlock_lock_at(struct spinlock *lock, const char *file, const char *func,
                      int line)
{
    preempt_disable();
    acquire_debug(lock, file, func, line);
}

void spinlock_lock_irq_at(struct spinlock *lock, const char *file,
                          const char *func, int line)
{
    interrupt_disable();
    preempt_disable();
    acquire_debug(lock, file, func, line);
}

unsigned long spinlock_lock_irqsave_at(struct spinlock *lock, const char *file,
                                       const char *func, int line)
{
    unsigned long flags;
    flags = interrupt_save();
    preempt_disable();
    acquire_debug(lock, file, func, line);
    return flags;
}

#else /* CONFIG_DEBUG && CONFIG_DEBUG_SPINLOCK */

bool spinlock_trylock(struct spinlock *lock)
{
    int ret;
    preempt_disable();
    ret = try_lock(lock);
    if (ret)
        return true;
    preempt_enable();
    return false;
}

void spinlock_lock(struct spinlock *lock)
{
    preempt_disable();
    acquire(lock);
}

void spinlock_lock_irq(struct spinlock *lock)
{
    interrupt_disable();
    preempt_disable();
    acquire(lock);
}

unsigned long spinlock_lock_irqsave(struct spinlock *lock)
{
    unsigned long flags;
    flags = interrupt_save();
    preempt_disable();
    acquire(lock);
    return flags;
}

#endif /* CONFIG_DEBUG && CONFIG_DEBUG_SPINLOCK */

void spinlock_unlock(struct spinlock *lock)
{
    release(lock);
    preempt_enable();
}

void spinlock_unlock_irq(struct spinlock *lock)
{
    release(lock);
    interrupt_enable();
    preempt_enable();
}

void spinlock_unlock_irqrestore(struct spinlock *lock, unsigned long flags)
{
    release(lock);
    interrupt_restore(flags);
    preempt_enable();
}

void spinlock_init(struct spinlock *lock, const char *name)
{
    lock->name = name;
    lock->owner = NULL;
#if CONFIG_SMP
    atomic_store(&lock->locked, 0);
#else
    lock->locked = false;
#endif /* CONFIG_SMP */

#if CONFIG_DEBUG && CONFIG_DEBUG_SPINLOCK
    lock->file = NULL;
    lock->func = NULL;
    lock->line = 0;
#endif
}

bool spinlock_is_locked(struct spinlock *lock)
{
#if CONFIG_SMP
    return atomic_load(&lock->locked) != 0;
#else
    return lock->locked;
#endif /* CONFIG_SMP */
}