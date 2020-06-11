#pragma once

namespace mv
{
	enum class UpdateStage {
		// fixed update
		behaviour, // free to modify any aspect of the entity
		prephysics, // gridspace is updated during this stage, any gridspace related functions are disabled, transform is readonly
		physics, // free to modify any aspect of the entity
		postphysics, // gridspace is updated during this stage, any gridspace related functions are disabled, transform is readonly
		// frame update
		prerender, // called after the update, deltatime is variable frame time, render components transforms are calculated
		render // deltatime is variable frame time, has a render function that is called after the update
	};
}
