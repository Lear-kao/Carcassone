#include "../header/Carcassonne.h"

char nbMeepleAbbaye( struct Grid *grille)
/* 
Compter le nombre de meeple d'une certaine abbaye.
Pour chaques tuiles vérifie le meeple, si  il est de la bonne couleur et au bon endroit 
on renvoie le  nombre de meeple de couleur demandé sur l'abbaye.
*/
{
    char nbMeeple = 0;
    if (grille->top->tile != NULL && grille->tile->meeple->coulPlayer == coul_player && where_is_meeple(ABBAYES,*(grille->tile) == 1))
    {
        nbMeeple += 1;
    }
    if (grille->top->tile != NULL && grille->top->tile->meeple->coulPlayer == coul_player && where_is_meeple(ABBAYES,*(grille->top->tile) == 1))
    {
        nbMeeple += 1;
    }
    if (grille->bot->tile != NULL && grille->bot->tile->meeple->coulPlayer == coul_player && where_is_meeple(ABBAYES,*(grille->bot->tile) == 1))
    {
        nbMeeple += 1;
    }
    if (grille->left->tile != NULL && grille->left->tile->meeple->coulPlayer == coul_player && where_is_meeple(ABBAYES,*(grille->left->tile) == 1))
    {
        nbMeeple += 1;
    }
    if (grille->right->tile != NULL && grille->right->tile->meeple->coulPlayer == coul_player && where_is_meeple(ABBAYES,*(grille->right->tile) == 1))
    {
        nbMeeple += 1;
    }
    if (grille->bot->left->tile != NULL && grille->bot->left->tile->meeple->coulPlayer == coul_player && where_is_meeple(ABBAYES,*(grille->bot->left->tile) == 1))
    {
        nbMeeple += 1;
    }
    if (grille->bot->right->tile != NULL && grille->bot->right->tile->meeple->coulPlayer == coul_player && where_is_meeple(ABBAYES,*(grille->bot->right->tile) == 1))
    {
        nbMeeple += 1;
    }
    if (grille->top->left->tile != NULL && grille->top->left->tile->meeple->coulPlayer == coul_player && where_is_meeple(ABBAYES,*(grille->top->left->tile) == 1))
    {
        nbMeeple += 1;
    }
    if (grille->top->right->tile != NULL && grille->top->right->tile->meeple->coulPlayer == coul_player && where_is_meeple(ABBAYES,*(grille->top->right->tile) == 1))
    {
        nbMeeple += 1;
    }
    return nbMeeple;
}

char nbMeepleVille( struct Grid *grille, int coul_player)
{
    //pour l'instant la fonction ne fait que compter le nombre de meeple sans discinction
    char cmpt = 0;
    if (grille->marquer == v_marquer) return 0;
    if(grille->tile->meeplePlace>0 ) cmpt = 1;
    grille->marquer = v_marquer;
    if( grille->tile->middle == VILLE )
    {
        if(grille->tile->top == VILLE)
        {
            cmpt += nbMeepleVille(grille->top,coul_player);
        }
        if(grille->tile->right == VILLE)
        {
            cmpt += nbMeepleVille(grille->right,coul_player);
            
        }
        if(grille->tile->left == VILLE)
        {
            cmpt += nbMeepleVille(grille->left,coul_player);
        }
        
        if(grille->tile->bot == VILLE)
        {
            cmpt += nbMeepleVille(grille->bot,coul_player);
        }
    }
    return cmpt;
}

void give_point(char *list_meeple_player, struct list_player *list, char point)
/* 
Prend en  paramètre :
-la liste du nombre de meeple par joueur trouvée dans le parcour de la ville/route/abbaye
-la liste des joeurs
-le nombre de point
*/
{
    int max_L = max(list_meeple_player);
    for( int i = 0; i < nbPlayers; i++)
    {
        if(list_meeple_player[i] == max) list->*player[i]->points += point; 
    }
    
}

char where_is_meeple(  int type , struct Tile tile)
{
    switch (type)
    {
    case BLASON:
    case VILLE:
        switch (tile->meeplePlace)
        {
        case RIGHT:
            if (tile->right == VILLE) return 1;
            break;
        case TOP:
            if (tile->top == VILLE) return 1;
            break;
        case LEFT:
            if (tile->left == VILLE) return 1;
            break;
        case BOT:
            if (tile->bot == VILLE) return 1;
            break;
        }
        break;
    case ABBAYES:
        switch (tile->meeplePlace)
        {
        case RIGHT:
            if (tile->right == ABBAYES) return 1;
            break;
        case TOP:
            if (tile->top == ABBAYES) return 1;
            break;
        case LEFT:
            if (tile->left == ABBAYES) return 1;
            break;
        case BOT:
            if (tile->bot == ABBAYES) return 1;
            break;
        }
        break;
    case ROUTE:
        switch (tile->meeplePlace)
        {
        case RIGHT:
            if (tile->right == ROUTE) return 1;
            break;
        case TOP:
            if (tile->top == ROUTE) return 1;
            break;
        case LEFT:
            if (tile->left == ROUTE) return 1;
            break;
        case BOT:
            if (tile->bot == ROUTE) return 1;
            break;
        }
        break;
    default:
        break;
    }
    return 0;
}

int max(char *list)
/* 
Prend en paramètre une liste d'entiers et renvoie le plus grand
*/
{
    int max = list[0];
    for( int i = 0; i<nbPlayers; i++)
    {
        if (max < list[i]) max = list[i];
    }
    return max;
}