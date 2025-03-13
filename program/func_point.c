#include "../header/Carcassonne.h"



char isFinishedAbbaye(struct Grid *grille, char finJeu)
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
    if (grille->top->right->tile != NULL) point++;
    if (finJeu!= 0) return point;
    if (point != 8) return 0;
    return point;
}


char isFinishedCity( struct Grid *grille, char finJeu , char *unfinished, char v_marquer)
/* 
Compter les points villes
Elle vérifie chaque tuiles ville conntecté à celle envoyé, si chacune de celles-ci sont complètes (on ne peut plus ajouter de 
tuiles villes) la fonction renvoie les points si finJeu == 0 et la  ville est complète ou si finJeu == 1.
On entre en  paramètre une grille, un char idiquant si on compte les points de fin de jeu ou non, 
    un charactère servant de marquer pour savoir si la ville est complète ou non et un charadctère 
    pour connaitre la valeur du marquer (-1 ou 1)
*/
{
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
                cmpt += isFinishedCity(grille->top,finJeu,unfinished,v_marquer);
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
                cmpt += isFinishedCity(grille->right,finJeu,unfinished,v_marquer);
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
                cmpt += isFinishedCity(grille->left,finJeu,unfinished,v_marquer);
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
                cmpt += isFinishedCity(grille->bot,finJeu,unfinished,v_marquer);
            }
        }
    }
    if (*unfinished == 1 && finJeu == 0) return 0;
    return cmpt;
}