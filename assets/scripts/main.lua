json = require("assets.scripts.internal.json")


registered_functions =
{
	"day_night_cycle_init",
	"get_scenes"
}

function test_file_io()
	local file = io.open("test.txt", "w")

	local j = json.decode('{"hello":"World!"}')
	
	print(j.hello)

	local x = json.encode(j)

	print(x)
	file:write(x)
	file:close()
end


function print(str)
	io.write("[LUA] " .. str .. "\n")
end

function get_scenes()


--Should a scene be a single file? It might be easier to manage

--An entity should have the option of containing params such as:
--
--Player { position[0,0,0] }
--PointLight {position: [10,10,10], ambient:[.1,.1,.1], specular:[.8,.9,1], activeRange:20}
--
--



--multiline string starts and ends with [[ and ]]
scenes = [[
        [
        [ "Player",
          "LightTest",
          "PlayerTestObject",
          "EnemyTestObject",
          "FloorObject",
          "ParticleTest" ],
        [ "Player",
          "LightTest",
          "PlayerTestObject",
          "EnemyTestObject",
          "ParticleTest" ],
        [ "Player",
          "LightTest",
          "PlayerTestObject",
          "EnemyTestObject",
          "CubeTrigger",
          "FloorObject",
          "ParticleTest" ],
        [ "Player",
          "LightTest",
          "PlayerTestObject",
          "FloorObject",
          "error!" ],
        [ "Player",
          "LightTest",
          "PlayerTestObject" ]
          ]
]]

    return json.decode(scenes)

end

function day_night_cycle_init()

test_file_io()
    print("Running day_night_cycle_init.")

	return {
		speed = .1,
		fast_speed = 20,
		night_light_diffuse = vec3f:new(.05, .2, .3), 
		day_light_diffuse = vec3f:new(.8, .5, .5)
	};

end



