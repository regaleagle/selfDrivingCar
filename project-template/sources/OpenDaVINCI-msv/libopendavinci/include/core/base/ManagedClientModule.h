/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2008 - 2015 Christian Berger, Bernhard Rumpe
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef OPENDAVINCI_BASE_MANAGEDCLIENTMODULE_H_
#define OPENDAVINCI_BASE_MANAGEDCLIENTMODULE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/Breakpoint.h"
#include "core/base/ClientModule.h"
#include "core/data/TimeStamp.h"
#include "core/exceptions/Exceptions.h"

#include "context/base/Clock.h"
#include "context/base/ControlledTimeFactory.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This class manages the local module:
         *  - unsupervised distributed execution
         *  - supervised distributed execution
         *
         * @See ConferenceClientModule
         */
        class OPENDAVINCI_API ManagedClientModule : public core::base::ClientModule, public core::base::Breakpoint
        {
            private:
                friend class ConferenceClientModule;

                /**
                 * Private constructor to not allow any other subclasses than ConferenceClientModule to instantiate an object.
                 *
                 * @param argc Number of command line arguments.
                 * @param argv Command line arguments.
                 * @param name Name of this module. This parameter is necessary for identifying the corresponding parts in the configuration.
                 * @throw InvalidArgumentException if the signal handler could not be registered.
                 */
                ManagedClientModule(const int32_t &argc, char **argv, const string &name) throw (core::exceptions::InvalidArgumentException);

            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                ManagedClientModule(const ManagedClientModule&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                ManagedClientModule& operator=(const ManagedClientModule&);

            public:
                virtual ~ManagedClientModule();

                /**
                 * @return Start time of the current execution cycle.
                 */
                const core::data::TimeStamp getStartOfCurrentCycle() const;

                /**
                 * @return End time of the last execution cycle.
                 */
                const core::data::TimeStamp getStartOfLastCycle() const;

            protected:
                /**
                 * This method is called right before the body is executed.
                 */
                virtual void setUp() = 0;

                /**
                 * This method is called after returning from body.
                 */
                virtual void tearDown() = 0;

                /**
                 * This method contains the real module body.
                 *
                 * @return The exit code of the real body.
                 */
                virtual core::base::ModuleState::MODULE_EXITCODE body() = 0;

                virtual core::base::ModuleState::MODULE_EXITCODE runModuleImplementation();

                virtual void reached();

            private:
                virtual void wait();

                virtual void DMCPconnectionLost();

                core::base::ModuleState::MODULE_EXITCODE runModuleImplementation_ManagedLevel_None();
                void wait_ManagedLevel_None();

                core::base::ModuleState::MODULE_EXITCODE runModuleImplementation_ManagedLevel_Pulse();
                void wait_ManagedLevel_Pulse();
                void reached_ManagedLevel_Pulse();

                core::base::ModuleState::MODULE_EXITCODE runModuleImplementation_ManagedLevel_Pulse_Shift();
                void wait_ManagedLevel_Pulse_Shift();
                void reached_ManagedLevel_Pulse_Shift();

                core::base::ModuleState::MODULE_EXITCODE runModuleImplementation_ManagedLevel_Pulse_Time();
                void wait_ManagedLevel_Pulse_Time();
                void reached_ManagedLevel_Pulse_Time();

                core::base::ModuleState::MODULE_EXITCODE runModuleImplementation_ManagedLevel_Pulse_Time_Ack();
                void wait_ManagedLevel_Pulse_Time_Ack();
                void reached_ManagedLevel_Pulse_Time_Ack();

                /**
                 * This method calculates the waiting time in microseconds
                 * to complete this execution cycle. Furthermore, it updates
                 * RuntimeStatistics and local profiling data.
                 *
                 * @return waiting time in microseconds.
                 */
                uint32_t getWaitingTimeAndUpdateRuntimeStatistics();

                /**
                 * This method is used to log the time consumption (load)
                 * for this module into a profiling file.
                 */
                void logProfilingData(const core::data::TimeStamp &current, const core::data::TimeStamp &lastCycle, const float &freq, const long &lastWaitTime, const long &timeConsumptionCurrent, const long &nominalDuration, const long &waitingTimeCurrent, const int32_t &cycleCounter);

            private:
                core::data::TimeStamp m_startOfCurrentCycle;
                core::data::TimeStamp m_startOfLastCycle;
                core::data::TimeStamp m_lastCycle;
                long m_lastWaitTime;
                int32_t m_cycleCounter;
                ofstream *m_profilingFile;

                bool m_firstCallToBreakpoint_ManagedLevel_Pulse;

                context::base::Clock m_time;
                context::base::ControlledTimeFactory *m_controlledTimeFactory;
        };

    }
} // core::base

#endif /*OPENDAVINCI_BASE_MANAGEDCLIENTMODULE_H_*/
