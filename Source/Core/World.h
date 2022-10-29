#pragma once

#include <unordered_map>

class Mesh;
class Shader;

#include "Window/InputController.h"

class World : public InputController
{
	public:
		World();
		virtual ~World() {};
		virtual void Init() {};
		virtual void FrameStart() {};
		virtual void Update(float deltaTimeSeconds) {};
		virtual void FrameEnd() {};

		virtual void Run() final;
		virtual void Pause() final;
		virtual void Exit() final;
		virtual void ShowFPS() final;
		virtual double GetLastFrameTime() final;

	private:
		void ComputeFrameDeltaTime();
		void LoopUpdate();

	private:
		double previousTime;
		double elapsedTime;
		double deltaTime;
		bool paused;
		bool shouldClose;

		double currentTime = 0.0, timeDiff, prevTime = 0.0;
		double counter = 0;
};