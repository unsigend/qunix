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

#ifndef _QUNIX_SPINLOCK_H_
#define _QUNIX_SPINLOCK_H_

#include <kernel/config.h>
#if CONFIG_SMP
#include <kernel/atomic.h>
#endif /* CONFIG_SMP */
#include <kernel/stdbool.h>

struct cpu;

struct spinlock {
#if CONFIG_SMP
    atomic_t locked;
#else
    bool locked;
#endif /* CONFIG_SMP */
    const char *name;
    struct cpu *owner;
#if CONFIG_DEBUG && CONFIG_DEBUG_SPINLOCK
    const char *file;
    const char *func;
    int line;
#endif
};

extern void spinlock_init(struct spinlock *lock, const char *name);
extern bool spinlock_is_locked(struct spinlock *lock);

#if CONFIG_DEBUG && CONFIG_DEBUG_SPINLOCK
extern bool spinlock_trylock_at(struct spinlock *lock, const char *file,
                                const char *func, int line);
extern void spinlock_lock_at(struct spinlock *lock, const char *file,
                             const char *func, int line);
extern void spinlock_lock_irq_at(struct spinlock *lock, const char *file,
                                 const char *func, int line);
extern unsigned long spinlock_lock_irqsave_at(struct spinlock *lock,
                                              const char *file,
                                              const char *func, int line);

#define spinlock_trylock(lock)                                                 \
    spinlock_trylock_at(lock, __FILE__, __func__, __LINE__)
#define spinlock_lock(lock) spinlock_lock_at(lock, __FILE__, __func__, __LINE__)
#define spinlock_lock_irq(lock)                                                \
    spinlock_lock_irq_at(lock, __FILE__, __func__, __LINE__)
#define spinlock_lock_irqsave(lock)                                            \
    spinlock_lock_irqsave_at(lock, __FILE__, __func__, __LINE__)
#else
extern bool spinlock_trylock(struct spinlock *lock);
extern void spinlock_lock(struct spinlock *lock);
extern void spinlock_lock_irq(struct spinlock *lock);
extern unsigned long spinlock_lock_irqsave(struct spinlock *lock);
#endif /* CONFIG_DEBUG && CONFIG_DEBUG_SPINLOCK */

extern void spinlock_unlock(struct spinlock *lock);
extern void spinlock_unlock_irq(struct spinlock *lock);
extern void spinlock_unlock_irqrestore(struct spinlock *lock,
                                       unsigned long flags);

#endif /* _QUNIX_SPINLOCK_H_ */