#pragma once

#include "protocol/debugger.h"

#include "GameInterfaces.h"

#include "BreakpointManager.h"
#include "RuntimeState.h"

#include <mutex>

namespace DarkId::Papyrus::DebugServer
{
	
	using namespace RE::BSScript::Internal;

	class DebugExecutionManager
	{
		enum class DebuggerState
		{
			kRunning = 0,
			kPaused,
			kStepping
		};

		std::mutex m_instructionMutex;
		bool m_closed = false;

		Protocol* m_protocol;
		RuntimeState* m_runtimeState;
		BreakpointManager* m_breakpointManager;

		DebuggerState m_state = DebuggerState::kRunning;
		UInt32 m_currentStepStackId = 0;
		Debugger::StepType m_currentStepType = Debugger::StepType::STEP_IN;
		RE::BSScript::StackFrame* m_currentStepStackFrame;
	public:
		explicit DebugExecutionManager(Protocol* protocol, RuntimeState* runtimeState,
									   BreakpointManager* breakpointManager)
			: m_protocol(protocol), m_runtimeState(runtimeState), m_breakpointManager(breakpointManager),
			  m_currentStepStackFrame(nullptr)
		{
		}

		void Close();
		void HandleInstruction(CodeTasklet* tasklet, CodeTasklet::OpCode opCode);
		bool Continue();
		bool Pause();
		bool Step(UInt32 stackId, Debugger::StepType stepType);
	};
}
