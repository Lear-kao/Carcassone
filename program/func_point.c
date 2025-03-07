#include "../header/Carcassonne.h"


char isFinishedAbbaye(struct Grid grille, char finJeu)
/* 
Compter les points abbaye.
La fonction vérifie si l'abaye est complète avec une simple  vérification des tuiles autours.
Elle vérifie chaque tuiles autour et  pour  chaques tuiles compte les points, si finJeu est != 0 (la partie est finie)
la fonction envoie les points même si elle n'est pas complètement entourée, sinon elle envoie 0 si,l'abbaye n'est
pas complètement entourée.
*/
{
    char point = 0;
    if (grille.top->tile != NULL) point++;
    if (grille.bot->tile != NULL) point++;
    if (grille.left->tile != NULL) point++;
    if (grille.right->tile != NULL) point++;
    if (grille.bot->left->tile != NULL) point++;
    if (grille.bot->right->tile != NULL) point++;
    if (grille.top->left->tile != NULL) point++;
    if (grille.top->right->tile != NULL) point++;
    if (finJeu!= 0) return point;
    if (point != 8) return 0;
    return point;
}


char isFinishedCity( struct Grid *grille, char finJeu , char *m)
/* 
Compter les points villes
Elle vérifie chaque tuiles ville conntecté à celle envoyé, si chacune de celles-ci sont complètes (on ne peut plus ajouter de 
tuiles villes) la fonction 
*/
{
    if (grille->marquer == 1) return 0;
    char cmpt = 0;
    grille->marquer = 1;
    if(grille->tile->top == VILLE)
    {
        if(grille->top == NULL)
        {
            *m = 1;
        }
        else
        {
            cmpt += isFinishedAbbaye(grille->top,finJeu,m);
        }
    }
    if(grille->tile->right == VILLE)
    {
        if(grille->right == NULL)
        {
            *m = 1;
        }
        else
        {
            cmpt += isFinishedAbbaye(grille->top,finJeu,m);
        }
    }
    if(grille->tile->left == VILLE)
    {
        if(grille->left == NULL)
        {
            *m = 1;
        }
        else
        {
            cmpt += isFinishedAbbaye(grille->top,finJeu,m);
        }
    }
    if(grille->tile->bot == VILLE)
    {
        if(grille->bot == NULL)
        {
            *m = 1;
        }
        else
        {
            cmpt += isFinishedAbbaye(grille->top,finJeu,m);
        }
    }

}