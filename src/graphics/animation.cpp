#include "graphics/animation.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "resources/md5anim.hpp"

namespace tec {
	void Animation::UpdateAnimation(const double delta) {
		if (this->frame_count < 1) {
			return;
		}

		this->animation_time += delta;

		while (this->animation_time > this->frame_duration) {
			this->animation_time -= this->frame_duration;
		}
		while (this->animation_time < 0.0f) {
			this->animation_time += this->frame_duration;
		}

		// Figure out which frame we're on
		double frame_number = this->animation_time * frame_rate;
		int frame_index0 = (int)floor(frame_number);
		int frame_index1 = (int)ceil(frame_number);
		frame_index0 = frame_index0 % frame_count;
		frame_index1 = frame_index1 % frame_count;

		double fInterpolate = fmod(this->animation_time, this->frame_duration);

		if (this->animation_file) {
			auto frame_skeleton = this->animation_file->InterpolateSkeletons(
				frame_index0, frame_index1, fInterpolate);
			this->animation_matrices.assign(frame_skeleton.bone_matricies.begin(), frame_skeleton.bone_matricies.end());
		}
	}

	void Animation::SetAnimationFile(std::shared_ptr<MD5Anim> file) {
		if (file) {
			this->animation_file = file;
			this->frame_count = this->animation_file->GetFrameCount();
			this->frame_rate = static_cast<float>(this->animation_file->GetFrameRate());
			this->frame_duration = 1.0f / this->frame_rate * this->frame_count;

			auto frame_skeleton = this->animation_file->InterpolateSkeletons(
				0, 1, 0.0f);
			this->animation_matrices.assign(frame_skeleton.bone_matricies.begin(), frame_skeleton.bone_matricies.end());
		}
	}

}