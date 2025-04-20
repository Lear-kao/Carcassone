#include "../header/Carcassonne.h"



int main()
{
    int gameRuns = 1;       // Vaut 1 le temps que le jeu doit continuer, 0 sinon.
    unsigned char nbHumanPlayers = 0; // Nombre de joueurs humain
    unsigned char nbBotPlayers = 0;   // Nombre de bots
    struct DLList;
    printf("Texte de bienvenu");

    while (gameRuns)
    {
        // ----Phase 1 : Paramètrage de la partie----
        start_game()

        // ----------------------------------------
        // ----Phase 2 : Lancement de la partie----
        // ----------------------------------------
        
        struct list_player *p_list;
        struct Grid *grid;
        struct Stack *stack = start_game(&p_list, &grid);        
        // ---------------------------
        // ----Phase 3 : La partie----
        // ---------------------------
        while (is_stack_not_empty(stack))
        {
            for (char i = 0; i < nbPlayers; i++) 
            {
                //player_turn(i,p_list,stack,&grid);
            }
        }

        // -------------------------------
        // ----Phase 4 : Fin de partie----
        // -------------------------------
        end_game_points_counter(*p_list);
        printf("voulez-vous continuer?(0:non/1:oui)\n");
        free_Grid(&grid);
        scanf("%d",&gameRuns);
    }
}
