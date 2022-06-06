#include "main.h"

#include "Game.h"

int main(int argc, char* argv[], char* env[])
{
    ((void)argc);
    ((void)argv);
    ((void)env);

    Game g("config.txt");
    g.run();

    return 0;
}
