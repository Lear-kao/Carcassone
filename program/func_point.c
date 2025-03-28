#include "../header/Carcassonne.h"



char isFinishedAbbaye( struct Grid *grille)
/* 
Compter les points abbaye.
La fonction vérifie si l'abaye est complète avec une simple  vérification des tuiles autours.
Elle vérifie chaque tuiles autour et  pour  chaques tuiles compte les points, si finJeu est != 0 (la partie est finie)
la fonction envoie les points même si elle n'est pas complètement entourée, sinon elle envoie 0 si,l'abbaye n'est
pas complètement entourée.
*/
{
    char point = 0;
    if (grille->top->tile != NULL) point++;
    if (grille->bot->tile != NULL) point++;
    if (grille->left->tile != NULL) point++;
    if (grille->right->tile != NULL) point++;
    if (grille->bot->left->tile != NULL) point++;
    if (grille->bot->right->tile != NULL) point++;
    if (grille->top->left->tile != NULL) point++;
    if (grille->top->right->tile != NULL ) point++;
    if (finJeu!= 0) return point;
    if (point != 8) return 0;
    return point;
}

char count_point_city( struct Grid *grille, enum places a )
{
    char point = 2;
    char unfinished;
    grille->marquer = v_marquer;
    if (a < 4){
        if(grille->a != NULL)
        {
            point += isFinishedCity(grille->a, &unfinished);
        }
        else unfinished = 1;
    }
    else
    {
        point = isFinishedCity(grille,&unfinished);
    }
    if( finJeu == 1 || unfinished  == 0)
    return point;
    return 0;
}

char isFinishedCity( struct Grid *grille, char *unfinished)
/* 
Compter les points villes
Elle vérifie chaque tuiles ville conntecté à celle envoyé, si chacune de celles-ci sont complètes (on ne peut plus ajouter de 
tuiles villes) la fonction renvoie les points si finJeu == 0 et la  ville est complète ou si finJeu == 1.
On entre en  paramètre une grille, un char idiquant si on compte les points de fin de jeu ou non, 
    un charactère servant de marquer pour savoir si la ville est complète ou non et un charadctère 
    pour connaitre la valeur du marquer (-1 ou 1)
*/
{
    if( is_a_potential_tile(grille->tile) == 1)
    {
        *unfinished = 1;
        return 0;
    }
    if (grille->marquer == v_marquer) return 0;
    char cmpt = 2;

    grille->marquer = v_marquer;
    if( grille->tile->middle == VILLE )
    {
        if(grille->tile->top == VILLE)
        {
            if(grille->top == NULL)
            {
                *unfinished = 1;
            }
            else
            {
                cmpt += isFinishedCity(grille->top,unfinished);
            }
        }
        if(grille->tile->right == VILLE)
        {
            if(grille->right == NULL)
            {
                *unfinished = 1;
            }
            else
            {
                cmpt += isFinishedCity(grille->right,unfinished);
            }
        }
        if(grille->tile->left == VILLE)
        {
            if(grille->left == NULL)
            {
                *unfinished = 1;
            }
            else
            {
                cmpt += isFinishedCity(grille->left,unfinished);
            }
        }
        if(grille->tile->bot == VILLE)
        {
            if(grille->bot == NULL)
            {
                *unfinished = 1;
            }
            else
            {
                cmpt += isFinishedCity(grille->bot,unfinished);
            }
        }
    }
    if (*unfinished == 1 && finJeu == 0) return 0;
    return cmpt;
}

char isFinishedRoad(struct Grid *grille, char *unfinished)
/* 
Commentaire à faire
!!! unfinished initialisé à 1
*/
{
    if( is_a_potential_tile(grille->tile) == 1)
    {
        return 0;
    }
    if( grille->marquer == v_marquer)
    {
        *unfinished = 0;
        return 0;
    }
    grille->marquer = v_marquer;
    char cmp = 1;
    if ( grille->tile->bot == ROUTE && grille->bot !=NULL )
    {
        cmp += isFinishedRoad(grille->bot,unfinished);
    }
    if ( grille->tile->left == ROUTE && grille->left !=NULL )
    {
        cmp += isFinishedRoad(grille->left,unfinished);
    }
    if ( grille->tile->top == ROUTE && grille->top !=NULL )
    {
        cmp += isFinishedRoad(grille->top,unfinished);
    }
    if ( grille->tile->right == ROUTE && grille->right !=NULL )
    {
        cmp += isFinishedRoad(grille->right,unfinished);
    }
    if(grille->tile->middle == VILLAGE)
    {
        *unfinished = 0;
        return 1;
    }
    if (*unfinished == 1 && finJeu == 0)
    {
        return 0;
    }
    return cmp;
}

void pointPlacedTile(struct Grid *justPlaced, struct list_player *listPlayer)
{
    char point;
    char middle_type;
    switch (justPlaced->tile->middle)
    {
        VILLE:
            char a = count_point_city(justPlaced,4);
            char list_meeple[nbPlayers];
            for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i);
            give_point(list_meeple,listPlayer,a);
            break;
        ABBAYES:
            char a = count_point_city(justPlaced,4);
            char list_meeple[nbPlayers];
            //for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleAbbaye(justPlaced, i);
            give_point(list_meeple,listPlayer,a);
            break;
        ROUTE:
    }
}

