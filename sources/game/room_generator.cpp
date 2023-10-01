#include "../../headers/game/room_generator.h"

namespace game
{
    RoomGenerator::RoomGenerator(int thickness) {
        wallThickness = thickness;
    }

    Scene* RoomGenerator::generateRoomScene() {
        std::vector<std::vector<int>> roomData;
        roomData.push_back({0, 0, 292, 300});
        roomData.push_back({0, 300, 292, 100});
        roomData.push_back({292, 0, 307, 190});
        roomData.push_back({292, 190, 223, 209});
        roomData.push_back({516, 190, 83, 209});
        roomData.push_back({0, 300, 58, 100});
        roomData.push_back({58, 300, 175, 100});
        roomData.push_back({233, 300, 58, 100});

        int xOffset = 1080/2 - houseWidth/2;
        int yOffset = 720/2 - houseHeight/2;

        Scene* resultingScene = new Scene{};
        for (auto data : roomData)
        {
            int x1 = data[0];
            int y1 = data[1];
            int x2 = data[2] + data[0];
            int y2 = data[3] + data[1];

            Vector2D startUpper{xOffset + x1, yOffset + y1};
            Vector2D endUpper{xOffset + x2, yOffset + y1+wallThickness};

            Vector2D startLower{xOffset + x1, yOffset + y2-wallThickness};
            Vector2D endLower{xOffset + x2, yOffset + y2+wallThickness};

            Vector2D startLeft{xOffset + x1, yOffset + y1};
            Vector2D endLeft{xOffset + x1+wallThickness, yOffset + y2};

            Vector2D startRight{xOffset + x2-wallThickness, yOffset + y1};
            Vector2D endRight{xOffset + x2, yOffset + y2};

            GameObject* upperWall = new GameObject{startUpper, endUpper};
            GameObject* lowerWall = new GameObject{startLower, endLower};
            GameObject* leftWall  = new GameObject{startLeft , endLeft};
            GameObject* rightWall = new GameObject{startRight, endRight};

            resultingScene->addObj(upperWall);
            resultingScene->addObj(lowerWall);
            resultingScene->addObj(leftWall);
            resultingScene->addObj(rightWall);
        }
        return resultingScene;
    }

}
