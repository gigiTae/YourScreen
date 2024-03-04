#pragma once

namespace ImpEngineModule::Physics
{
// Tag를 통해서 충돌 여부를 판단한다.
// Tag를 추가해야하는 경우에는 게임엔진 담당자에게 말해주세요~
enum class Tag
{
    Untagged   = 0,
    Obstacle   = 1,
    Background = 2,
    Hacking    = 3,
    Player     = 4,
    Attack     = 5,
    Camera       = 6,
    Detection  = 7,
    TAG8       = 8,
    TAG9       = 9,
    TAG10      = 10,
    TAG11      = 11,
    TAG12      = 12,
    TAG13      = 13,
    TAG14      = 14,
    TAG15      = 15,
    TAG16      = 16,
    TAG17      = 17,
    TAG18      = 18,
    TAG19      = 19,
    TAG20      = 20,
    TAG21      = 21,
    TAG22      = 22,
    TAG23      = 23,
    TAG24      = 24,
    TAG25      = 25,
    TAG26      = 26,
    TAG27      = 27,
    TAG28      = 28,
    TAG29      = 29,
    TAG30      = 30,
    TAG31      = 31,

    END = 32,
};


} // namespace ImpEngineModule::Physics
