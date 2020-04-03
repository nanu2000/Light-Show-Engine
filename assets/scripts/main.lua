registered_functions =
{
	"day_night_cycle_init"
}

function print(str)
	io.write("[LUA] " .. str .. "\n")
end

function day_night_cycle_init()

	return {
		speed = 1,
		fast_speed = 200,
		night_light_diffuse = vec3f:new(.1, .13, .5), 
		day_light_diffuse = vec3f:new(1, 1, 1)
	};

end