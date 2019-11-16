#version 330 core
in vec4 FragPosition_o;

uniform vec3 lightPosition;
uniform float farPlane;

void main() {
    // get distance between fragment and light source
    float lightDistance = length(FragPosition_o.xyz - lightPosition);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / farPlane;

    // Write this as modified depth
    gl_FragDepth = lightDistance;
}
