import QtQuick 2.2
import QtQuick.Particles 2.0

CustomParticle {
    property ShaderEffectSource normalmap
    property vector3d lightposition: Qt.vector3d(0.5, 0.5, 0.1)
    property color color

    vertexShader: "
        varying vec2 normalcoord;
        varying vec2 texcoord;
        varying float opacity;

        void main() {
            defaultMain();

            normalcoord = 0.5*(gl_Position.xy+vec2(1.0));
            texcoord = qt_TexCoord0;

            float t = (qt_Timestamp-qt_ParticleData.x)/qt_ParticleData.y;
            opacity = 1.0-t*t;
        }
    "

    fragmentShader: "
        uniform sampler2D normalmap;
        uniform vec3 lightposition;
        uniform vec4 color;

        varying vec2 normalcoord;
        varying vec2 texcoord;
        varying float opacity;

        void main() {
            vec3 normalcolor = texture2D(normalmap, normalcoord).rgb;
            vec3 lightdir = lightposition-vec3(texcoord, 0.0);
            float d = length(lightdir);

            vec3 n = normalize(normalcolor*2.0-1.0);
            vec3 l = normalize(lightdir);
            vec3 diffuse = color.rgb*color.a*max(dot(n, l), 0.0);

            gl_FragColor = 2.0*opacity*(color-2.0*d)*vec4(diffuse, 0.0);
        }
    "
}
