#pragma once

#include <memory>
#include <queue>

#include "types.hpp"
#include "game-state.hpp"
#include "physics-system.hpp"
#include "vcomputer-system.hpp"
#include "proto/components.pb.h"
#include "command-queue.hpp"

namespace tec {
	struct Controller;

	extern std::map<tid, std::function<void(const proto::Entity&, const proto::Component&)>> in_functors;

	class Simulation final : public EventQueue<KeyboardEvent>, public EventQueue<MouseBtnEvent>,
		public EventQueue<MouseMoveEvent>, public EventQueue < MouseClickEvent >,
		public CommandQueue < Simulation > {
	public:
		Simulation() : last_server_state_id(0) { }
		~Simulation() { }

		std::set<eid> Simulate(const double delta_time);

		void Interpolate(const double delta_time);

		// Used for testing only.
		void PopulateBaseState();

		PhysicsSystem& GetPhysicsSystem() {
			return this->phys_sys;
		}

		VComputerSystem& GetVComputerSystem() {
			return this->vcomp_sys;
		}

		GameState& GetClientState();

		void AddController(Controller* controller);

		void On(std::shared_ptr<KeyboardEvent> data);
		void On(std::shared_ptr<MouseBtnEvent> data);
		void On(std::shared_ptr<MouseMoveEvent> data);
		void On(std::shared_ptr<MouseClickEvent> data);

		void SetEntityState(proto::Entity& entity);
		void onServerStateUpdate(GameState&& new_frame);

	private:
		void onSetEntityState(const proto::Entity& entity);
		void onRemoveEntity(const eid entity_id);

		PhysicsSystem phys_sys;
		VComputerSystem vcomp_sys;

		std::queue<GameState> server_states;
		CommandList current_command_list;
		GameState client_state; // Current client (interpolated) state
		GameState base_state; // The base state we interpolate client state towards server_states.being()
		state_id_t last_server_state_id; // Stores the last server state id (no need for a mutex around server_states by caching).
		double interpolation_accumulator = 0.0;
		std::list<Controller*> controllers;
	};
}
