#pragma once

namespace mv
{
	enum class UpdateStage {
		// fixed update
		physics, // free to modify any aspect of the entity
		postphysics, // gridspace is updated during this stage, transform is readonly
		input, // collision is updated during this stage, transform is readonly
		behaviour, // free to modify any aspect of the entity
		// frame update
		prerender, // called after the update, deltatime is variable frame time, render components transforms are calculated
		render // deltatime is variable frame time, has a render function that is called after the update
	};
}
