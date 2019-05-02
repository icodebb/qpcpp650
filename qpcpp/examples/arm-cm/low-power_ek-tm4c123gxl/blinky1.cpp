//$file${.::blinky1.cpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: low-power.qm
// File:  ${.::blinky1.cpp}
//
// This code has been generated by QM 4.5.0 (https://www.state-machine.com/qm).
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
//$endhead${.::blinky1.cpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpcpp.h"
#include "low_power.h"
#include "bsp.h"

//Q_DEFINE_THIS_FILE

//$declare${AOs::Blinky1} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//${AOs::Blinky1} ............................................................
class Blinky1 : public QP::QActive {
public:
    static Blinky1 instance;

private:
    QP::QTimeEvt m_timeEvt1;
    uint8_t m_count;

public:
    Blinky1();

protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(inactive);
    Q_STATE_DECL(active);
    Q_STATE_DECL(off);
    Q_STATE_DECL(on);
};
//$enddecl${AOs::Blinky1} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 650U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 6.5.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${AOs::AO_Blinky1} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//${AOs::AO_Blinky1} .........................................................
QP::QActive * const AO_Blinky1 = &Blinky1::instance;
//$enddef${AOs::AO_Blinky1} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${AOs::Blinky1} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//${AOs::Blinky1} ............................................................
Blinky1 Blinky1::instance;
//${AOs::Blinky1::Blinky1} ...................................................
Blinky1::Blinky1()
: QActive(Q_STATE_CAST(&initial)),
  m_timeEvt1(this, TIMEOUT1_SIG, 1U)
{}

//${AOs::Blinky1::SM} ........................................................
Q_STATE_DEF(Blinky1, initial) {
    //${AOs::Blinky1::SM::initial}
    (void)e; /* unused parameter */
    subscribe(BTN_PRESSED_SIG);

    QS_FUN_DICTIONARY(&inactive);
    QS_FUN_DICTIONARY(&active);
    QS_FUN_DICTIONARY(&off);
    QS_FUN_DICTIONARY(&on);

    return tran(&inactive);
}
//${AOs::Blinky1::SM::inactive} ..............................................
Q_STATE_DEF(Blinky1, inactive) {
    QP::QState status_;
    switch (e->sig) {
        //${AOs::Blinky1::SM::inactive::BTN_PRESSED}
        case BTN_PRESSED_SIG: {
            status_ = tran(&active);
            break;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}
//${AOs::Blinky1::SM::active} ................................................
Q_STATE_DEF(Blinky1, active) {
    QP::QState status_;
    switch (e->sig) {
        //${AOs::Blinky1::SM::active}
        case Q_ENTRY_SIG: {
            BSP_tickRate1_on();
            m_count = 13U;
            m_timeEvt1.armX(2U, 2U);
            status_ = Q_RET_HANDLED;
            break;
        }
        //${AOs::Blinky1::SM::active}
        case Q_EXIT_SIG: {
            m_timeEvt1.disarm();
            status_ = Q_RET_HANDLED;
            break;
        }
        //${AOs::Blinky1::SM::active::initial}
        case Q_INIT_SIG: {
            status_ = tran(&on);
            break;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}
//${AOs::Blinky1::SM::active::off} ...........................................
Q_STATE_DEF(Blinky1, off) {
    QP::QState status_;
    switch (e->sig) {
        //${AOs::Blinky1::SM::active::off::TIMEOUT1}
        case TIMEOUT1_SIG: {
            status_ = tran(&on);
            break;
        }
        default: {
            status_ = super(&active);
            break;
        }
    }
    return status_;
}
//${AOs::Blinky1::SM::active::on} ............................................
Q_STATE_DEF(Blinky1, on) {
    QP::QState status_;
    switch (e->sig) {
        //${AOs::Blinky1::SM::active::on}
        case Q_ENTRY_SIG: {
            BSP_led1_on();
            status_ = Q_RET_HANDLED;
            break;
        }
        //${AOs::Blinky1::SM::active::on}
        case Q_EXIT_SIG: {
            BSP_led1_off();
            status_ = Q_RET_HANDLED;
            break;
        }
        //${AOs::Blinky1::SM::active::on::TIMEOUT1}
        case TIMEOUT1_SIG: {
            --m_count;
            //${AOs::Blinky1::SM::active::on::TIMEOUT1::[m_count>0]}
            if (m_count > 0) {
                status_ = tran(&off);
            }
            //${AOs::Blinky1::SM::active::on::TIMEOUT1::[else]}
            else {
                status_ = tran(&inactive);
            }
            break;
        }
        default: {
            status_ = super(&active);
            break;
        }
    }
    return status_;
}
//$enddef${AOs::Blinky1} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
