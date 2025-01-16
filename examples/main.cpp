#include "../cppjson.h"
#include <iostream>

int main()
{
    using namespace json;
    Json obj
    {
        {"best_club_ever", "FC Bayern München"},
        {"best_national_team", "Die Mannschaft"},
        {"best_players", Object{
                {"goalkeeper", "Manuel Neuer"},
                {"defender", "Jerome Boateng"},
                {"midfielder", "Joshua Kimmich"},
                {"forward", "Thomas Müller"}
            }
        },
        {"best_coaches", Array{
                "Jupp Heynckes",
                "Ottmar Hitzfeld"
            }
        }
    };
    std::cout << obj << std::endl;

    std::cout << R"(
    {
        "best_club_ever": "FC Bayern München",
        "champions_league_titles": 6,
    }
    )"_Json["champions_league_titles"] << std::endl;

    return 0;
} 