#include "Carcassonne.h"


void main()
{
    char gameRuns = 1;       // Vaut 1 le temps que le jeu doit continuer, 0 sinon.
    char nbHumanPlayers = 0; // Nombre de joueurs humain
    char nbBotPlayers = 0;   // Nombre de bots
    char nbPlayers = 0;      // Nombre de joueurs total
    char turnTraker = 0;     // Numéro du joueur dont c'est le tour.
    printf("Texte de bienvenu");

    while (gameRuns)
    {
        // ----Phase 1 : Paramètrage de la partie----
        char phase1 = 1;
        while (phase1)
        {
            printf("Entrez le nombre de joueurs humains : ");
            scanf("%u", &nbHumanPlayers);
            puts(); // pour le \n
            printf("Entrez le nombre de robots participant à la partie : ");
            scanf("%u", &nbBotPlayers);
            puts(); // pour le \n
            nbPlayers = nbHumanPlayers + nbBotPlayers;
            if ((nbPlayers) <= 6) // Phase 1 terminé
            {
                phase1 = 0;
            }
            else // boucle sur Phase 1
            {
                printf("Le nombre de joueurs et de bot doit être compris entre 0 et 6\n");
            }
        }

        // ----------------------------------------
        // ----Phase 2 : Lancement de la partie----
        // ----------------------------------------
        
        // Dans start game
        //struct **create_players_array(char nbPlayers);
        //struct Tile **create_tile_array(FILE * stream);
        //void shuffle(struct Tile * *tileArray, char size);
        //void array_to_stack(struct Tile * *tileArray, struct Stack * stack);
        // Fin dans start game

        start_game(nbHumanPlayers, nbBotPlayers);
        struct Player **playersArray = malloc(sizeof(struct Player) * nbPlayers)
        playersArray = create_players_array(char nbPlayers);
        
        // ---------------------------
        // ----Phase 3 : La partie----
        // ---------------------------
        show_grid();
        while (is_stack_not_empty(stack);)
        {
            for (char i = 0; i < nbPLayers; i++) 
            {
                player_turn(i);
                show_grid(grid);
            }
        }

        // -------------------------------
        // ----Phase 4 : Fin de partie----
        // -------------------------------
        
        char *end_game_points_counter(struct Grid *grid, struct Player nbPlayers);
    }
}