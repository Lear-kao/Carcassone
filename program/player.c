#include "../header/Carcassonne.h"

// ---------------
// ----Player-----
// ---------------

struct Player *init_player(int couleur)
{
    /*
        Innitialise l'objet player
    */
    struct Player *player=malloc(sizeof(struct Player));
    player->coulPlayer = couleur;  
    player->nbMeeple = NBMEEPLE_DEFAULT;
    player->points = 0;
    return player;
}

char is_meeple_on_player(struct Player *player)
{/*
    Player : L'objet Player

    return :
    - 0 si le joueur n'a aucun Meeple
    - 1 si il reste au moins un Meeple au Joueur
*/
    return player->nbMeeple;
}

struct list_player *init_player_list() // Axel
/*
    Crée une liste de pointeurs qui pointe sur un Player,
    un pointer sur NULL est ajouté à la fin pour faciliter
    les iterations sur la liste (condition d'arrêt)
*/
{
    struct list_player *list_players = (struct list_player*)malloc(sizeof(struct list_player));
    list_players->player = (struct Player**)malloc((nbPlayers + 1)*sizeof(struct Player*));
    for( int i = 0; i < nbPlayers; i++ )
    {
        list_players->player[i]=init_player(i);
    }
    list_players->player[nbPlayers] = NULL;
    return list_players;
}


void reset_points(struct Player *player) // Fait
/*
    Réinitialise les points du joueur
*/
{
    player->points = 0;
}

void reset_meeples(struct Player *player) // Fait
/*
    Réinitialise le compteur de Meeple du joueur
*/
{
    player->nbMeeple = NBMEEPLE_DEFAULT;
}