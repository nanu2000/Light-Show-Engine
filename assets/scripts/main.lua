registered_functions =
{
	"day_night_cycle_init"
}

function print(str)
	io.write("\n[LUA] " .. str)
end

function day_night_cycle_init()

	return {
		speed = 5,
		fast_speed = 100,
		night_light_diffuse = {x = .1, y = .13, z = .16}, 
		day_light_diffuse = {x = 1, y = .73, z = .74}
	};

end