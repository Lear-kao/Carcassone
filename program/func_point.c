#include "../header/Carcassonne.h"



char isFinishedAbbaye( struct Grid *grille)
/* 
Arguments:
    Un pointeur vers  la tuile de la Grille de Jeu ou se trouve l'Abbaye.
Retour:
    Le nombre de point pour la structure Abbaye.
Description:
    La  fonction va simplement parcourir les 8  tuiles autours de la tuile Abbaye et renvoyer:
        - Les 8 points  de la structure si celle-ci est complète.
        - 0 point si la structure est incomplète et que l'on est pas en fin de partie.
        - Un nombre de point correspondant au nombre de tuile autour si c'est la fin de la partie.
*/
{
    if(grille!=NULL && 
       grille->tile!=NULL && 
       grille->tile->right!=ABBAYES &&
       grille->tile->top!=ABBAYES &&
       grille->tile->left!=ABBAYES &&
       grille->tile->bot!=ABBAYES &&
       grille->tile->middle!=ABBAYES) return 0;

    char point = 0;
    if (grille->top->tile != NULL && !is_a_potential_tile(grille->top->tile)) point++;
    if (grille->bot->tile != NULL && !is_a_potential_tile(grille->bot->tile)) point++;
    if (grille->left->tile != NULL && !is_a_potential_tile(grille->left->tile)) point++;
    if (grille->right->tile != NULL && !is_a_potential_tile(grille->right->tile)) point++;
    if (grille->bot->left->tile != NULL && !is_a_potential_tile(grille->bot->left->tile)) point++;
    if (grille->bot->right->tile != NULL && !is_a_potential_tile(grille->bot->right->tile)) point++;
    if (grille->top->left->tile != NULL && !is_a_potential_tile(grille->top->left->tile)) point++;
    if (grille->top->right->tile != NULL && !is_a_potential_tile(grille->top->right->tile)) point++;
    if (finJeu!= 0) return point;
    if (point != 8) return 0;
    return point;
}

char count_point_city( struct Grid *grille, enum places a )
/* 
Arguments:
    Un pointeur vers  la tuile de la Grille de Jeu ou se trouve la Ville.
    L'endroit sur la tuile ou se trouve la Ville cherchée. 
Retour:
    Le nombre de point pour la structure Ville.
Description:
    La  fonction va inspecter la zone de la tuile ou est sensé se trouver la Ville puis apppeller 
    isFinishedCity qui va utiliser du parcours de graph pour vérifier et compter les points de la Ville,
    enfin cette fonction va retourner :
        - Les points  de la structure si celle-ci est complète.
        - 0 point si la structure est incomplète et que l'on est pas en fin de partie.
        - Un nombre de point correspondant au nombre de tuiles de la Ville si c'est la fin de la partie.
*/
{
    char point = 2;
    char unfinished=0;
    switch (a)
    {
        case RIGHT:
            grille->marquer = v_marquer;
            if(grille->right != NULL && (grille->tile->right==VILLE || grille->tile->right==BLASON))
            {
                point += isFinishedCity(grille->right, &unfinished);
            }
            else unfinished = 1;
            break;
        
        case TOP:
            grille->marquer = v_marquer;
            if(grille->top != NULL && (grille->tile->top==VILLE || grille->tile->top==BLASON))
            {
                point += isFinishedCity(grille->top, &unfinished);
            }
            else unfinished = 1;
            break;
    
        case LEFT:
            grille->marquer = v_marquer;
            if(grille->left != NULL && (grille->tile->left==VILLE || grille->tile->left==BLASON))
            {
                point += isFinishedCity(grille->left, &unfinished);
            }
            else unfinished = 1;
            break;
        
        case BOT:
            grille->marquer = v_marquer;
            if(grille->bot != NULL && (grille->tile->bot==VILLE || grille->tile->bot==BLASON))
            {
                point += isFinishedCity(grille->bot, &unfinished);
            }
            else unfinished = 1;
            break;
        
        case MIDDLE:
            if(grille->tile->middle==VILLE || grille->tile->middle==BLASON)
            {
                point = isFinishedCity(grille,&unfinished);
            }


            break;
    }
    v_marquer++;
    //printf("count_point_city -> unfinished = %d\n",unfinished);

    if(finJeu == 1)
        return point/2;
    if(unfinished  == 0)
        return point;
    else
        return 0;
}

char isFinishedCity( struct Grid *grille, char *unfinished)
/* 
Arguments:
    Un pointeur vers  la tuile de la Grille de Jeu ou se trouve la Ville a un instant t du parcour.
    Un pointeur vers un booléens initialisé à 0 correspondant à si oui ou non la structure Ville est finie.
Retour:
    Le nombre de point que vaut la structure.
Description:
    La fonction utilise un algorithme de parcour de graph avec marqueur pour parcourir la grille , 
    par défaut la route est considérée comme complète et dans le cas contraire unfinished passe à 1.
    La fonction renvoie:
        - Les points  de la structure si celle-ci est complète.
        - 0 point si la structure est incomplète et que l'on est pas en fin de partie.
        - Un nombre de point correspondant au nombre de tuiles de la Ville si c'est la fin de la partie.
Note:
    !!! unfinished initialisé à 1
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
    if( grille->tile->middle == VILLE || grille->tile->middle == BLASON)
    {
        if(grille->tile->top == VILLE || grille->tile->top == BLASON)
        {
            if(is_a_potential_tile(grille->top->tile))
            {
                *unfinished = 1;
            }
            else
            {
                if(grille->top->tile->bot==VILLE || grille->top->tile->bot==BLASON)
                cmpt += isFinishedCity(grille->top,unfinished);
            }
        }
        if(grille->tile->right == VILLE || grille->tile->right == BLASON)
        {
            if(is_a_potential_tile(grille->right->tile))
            {
                *unfinished = 1;
            }
            else
            {
                if(grille->right->tile->left==VILLE || grille->right->tile->left==BLASON)
                cmpt += isFinishedCity(grille->right,unfinished);
            }
        }
        if(grille->tile->left == VILLE || grille->tile->left == BLASON)
        {
            if(is_a_potential_tile(grille->left->tile))
            {
                *unfinished = 1;
            }
            else
            {
                if(grille->left->tile->right==VILLE || grille->left->tile->right==BLASON)
                cmpt += isFinishedCity(grille->left,unfinished);
            }
        }
        if(grille->tile->bot == VILLE || grille->tile->bot == BLASON)
        {
            if(is_a_potential_tile(grille->bot->tile))
            {
                *unfinished = 1;
            }
            else
            {
                if(grille->bot->tile->top==VILLE || grille->bot->tile->top==BLASON)
                cmpt += isFinishedCity(grille->bot,unfinished);
            }
        }
    }
    if (*unfinished == 1 && finJeu == 0) return 0;
    return cmpt;
}

char countPointRoad(struct Grid *grille, enum places start)
/* 
Arguments:
    Un pointeur vers  la tuile de la Grille de Jeu ou se trouve la Route.
    L'endroit sur la tuile ou se trouve la Route cherchée. 
Retour:
    Le nombre de point pour la structure Route.
Description:
    La  fonction va inspecter la zone de la tuile ou est sensé se trouver la Route puis apppeller 
    isFinishedRoad qui va utiliser du parcours de graph pour vérifier et compter les points de la Route,
    enfin cette fonction va retourner :
        - Les points  de la structure si celle-ci est complète.
        - 0 point si la structure est incomplète et que l'on est pas en fin de partie.
        - Un nombre de point correspondant au nombre de tuiles de la Route si c'est la fin de la partie.
*/
{
    char  point = 1;
    char unfinished = 0;
    switch(start)
    {
        case RIGHT:
            grille->marquer = v_marquer;
            if(grille->right != NULL && grille->tile->right==ROUTE)
            {
                point += isFinishedRoad(grille->right, &unfinished);
            }
            else unfinished = 1;
            break;
        case TOP:
            grille->marquer = v_marquer;
            if(grille->top != NULL && grille->tile->top==ROUTE)
            {
                point += isFinishedRoad(grille->top, &unfinished);
            }
            else unfinished = 1;
            break;
        case LEFT:
            grille->marquer = v_marquer;
            if(grille->left != NULL && grille->tile->left==ROUTE)
            {
                point += isFinishedRoad(grille->left, &unfinished);
            }
            else unfinished = 1;
            break;
        case BOT:
            grille->marquer = v_marquer;
            if(grille->bot != NULL && grille->tile->bot==ROUTE)
            {
                point += isFinishedRoad(grille->bot, &unfinished);
            }
            else unfinished = 1;
            break;
        
        case MIDDLE:
            if(grille->tile->middle==ROUTE)
            {
                point = isFinishedRoad(grille,&unfinished);
            }
    }

    printf("pointroute1=%d unfinishedroute1=%d\n",point,unfinished);

    v_marquer++;
    if (unfinished == 1 && finJeu == 0)
    {
        return 0;
    }
    return point;
}

char isFinishedRoad(struct Grid *grille, char *unfinished)
/* 
Arguments:
    Un pointeur vers  la tuile de la Grille de Jeu ou se trouve la Route a un instant t du parcour.
    Un pointeur vers un booléens initialisé à 1 correspondant à si oui ou non la structure Route est finie.
Retour:
    Le nombre de point que vaut la structure.
Description:
    La fonction utilise un algorithme de parcour de graph avec marqueur pour parcourir la grille , 
    par défaut la route est considérée comme incomplète et dans le cas contraire unfinished passe  à 0.
    La fonction renvoie:
        - Les points  de la structure si celle-ci est complète.
        - 0 point si la structure est incomplète et que l'on est pas en fin de partie.
        - Un nombre de point correspondant au nombre de tuiles de la Route si c'est la fin de la partie.
Note:
    !!! unfinished initialisé à 1
*/
{
    if( is_a_potential_tile(grille->tile) == 1)
    {
        *unfinished=1;
        return 0;
    }
    printf("exec2\n");
    if( grille->marquer == v_marquer) return 0;

    grille->marquer = v_marquer;
    char cmp = 1;

    if (grille->tile->middle == ROUTE)
    {
        if ( grille->tile->bot == ROUTE)
        {
            if(is_a_potential_tile(grille->bot->tile))
            {
                *unfinished=1;
                return 0;
            }
            else
            {
                if(grille->bot->tile->top==ROUTE)
                cmp += isFinishedRoad(grille->bot,unfinished);
            }
        }

        if ( grille->tile->top == ROUTE)
        {
            if(is_a_potential_tile(grille->top->tile))
            {
                *unfinished=1;
                return 0;
            }
            else
            {
                if(grille->top->tile->bot==ROUTE)
                cmp += isFinishedRoad(grille->top,unfinished);
            }
        }

        if ( grille->tile->right == ROUTE)
        {
            if(is_a_potential_tile(grille->right->tile))
            {
                *unfinished=1;
                return 0;
            }
            else
            {
                if(grille->right->tile->left==ROUTE)
                cmp += isFinishedRoad(grille->right,unfinished);
            }
        }

        if ( grille->tile->left == ROUTE)
        {
            if(is_a_potential_tile(grille->left->tile))
            {
                *unfinished=1;
                return 0;
            }
            else
            {
                if(grille->left->tile->right==ROUTE)
                cmp += isFinishedRoad(grille->left,unfinished);
            }
        }
    }
    if(grille->tile->middle == VILLAGE || grille->tile->middle == VILLE || grille->tile->middle == BLASON || grille->tile->middle == ABBAYES)
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
/* 
Argument:
    Un pointeur vers la dernière tuile  posée.
    La liste des joueurs.
Retour:
    Void.
Description:
    La  fonction prend en paramètre la dernière tuile posée et compte les points que permet d'acquérir cette tuiles sur d'après la structure présente au milieu.
    (nottament si une abbaye est présente dans les alentours).
*/
{
    char point;
    char unfinish_road=1;
    char list_meeple[nbPlayers];
    struct Grid *abbaye=searchAbbaye(justPlaced);
    if(abbaye!=NULL)
    {
        point=isFinishedAbbaye(abbaye);
        if(point>0)
        {
            for (int i = 0; i < nbPlayers; i++)
            {
                list_meeple[i] = nbMeepleAbbaye(abbaye, i+1);
            }
            printf("\n");
            give_point(list_meeple,listPlayer,point);
            removeMeepleAbbaye(abbaye);
        }
    }

    switch (justPlaced->tile->middle)
    {
        case VILLE:
        case BLASON:
            point = count_point_city(justPlaced,MIDDLE);
            if(point != 0)
            {
                for (int i = 0; i < nbPlayers; i++)
                {
                    list_meeple[i] = nbMeepleVille(justPlaced, i+1,MIDDLE);
                }
                printf("\n");
                give_point(list_meeple,listPlayer,point);
                removeMeepleVilleStart(justPlaced);
            }
            break;

        case ABBAYES:
            point = isFinishedAbbaye(justPlaced);
            if(point != 0)
            {
                for (int i = 0; i < nbPlayers; i++)
                {
                    list_meeple[i] = nbMeepleAbbaye(justPlaced, i+1);
                }
                printf("\n");
                give_point(list_meeple,listPlayer,point);
                removeMeepleAbbaye(justPlaced);
            }
            break;

        case ROUTE:
            point = countPointRoad(justPlaced,MIDDLE);
            if(point != 0)
            {
                for (int i=0; i < nbPlayers; i++)
                {
                    list_meeple[i] = countMeepleRoad(justPlaced,MIDDLE,i+1);
                }
                give_point(list_meeple,listPlayer,point);
                removeMeepleRoadStart(justPlaced);
            }
            break;
    }

    secondary_verification(justPlaced,listPlayer,justPlaced->tile->middle);
}

void give_point(char *list_meeple_player, struct list_player *list, char point)
/* 
Arguments:
    La liste  des joueurs à qui il faut donner des points (liste de 0 et  de 1).
    La liste des joueurs.
    Le nombre de point à donner.
Retour:
    Void.
Description:
    Donne à  chaque joueur qui est sensé en recevoir les points de la structure précédement vérifiée.
*/
{
    int max_L = max(list_meeple_player);

    if(max_L>0){
        for( int i = 0; i < nbPlayers; i++)
        {
            if(list_meeple_player[i] == max_L) list->player[i]->points += point; 
        }
    }
        
}

void secondary_verification(struct Grid *justPlaced, struct list_player *list, enum types middle)
/* 
Argument:
    Un pointeur vers la dernière tuile  posée.
    La liste des joueurs.
    Le type du milieu de la tuile.
Retour:
    Void.
Description:
    La  fonction prend en paramètre la dernière tuile posée et compte les points que permet d'acquérir cette tuiles sur d'après les structures présentes 
    a droite,en haut, à gauche, en bas.
*/
{
    char point;
    switch (justPlaced->tile->right)
    {
        case VILLE:
            if(BLASON == middle || VILLE == middle)break;
            point = count_point_city(justPlaced,RIGHT);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i+1,RIGHT);
                give_point(list_meeple,list,point);
                removeMeepleVilleStart(justPlaced);
            }
            break;
        
        case BLASON:
            if(BLASON == middle || VILLE == middle)break;
            point = count_point_city(justPlaced,RIGHT);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i+1,RIGHT);
                give_point(list_meeple,list,point);
                removeMeepleVilleStart(justPlaced);
            }
            break;
        case ROUTE:
            if(justPlaced->tile->right == middle || middle ==VILLAGE || middle == ABBAYES)break;
            point = countPointRoad(justPlaced,RIGHT);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleRoute(justPlaced, i);
                give_point(list_meeple,list,point);
                removeMeepleRoadStart(justPlaced);
            }
            break;
    }
    switch (justPlaced->tile->top)
    {
        case VILLE:
            if(BLASON == middle || VILLE == middle)break;
            point = count_point_city(justPlaced,TOP);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i+1,TOP);
                give_point(list_meeple,list,point);
            }
            break;
        
        case BLASON:
            if(BLASON == middle || VILLE == middle)break;
            point = count_point_city(justPlaced,TOP);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i+1,TOP);
                give_point(list_meeple,list,point);
                removeMeepleVilleStart(justPlaced,TOP)
            }
            break;
        case ROUTE:
            if(justPlaced->tile->top == middle || middle == VILLAGE || middle == ABBAYES)break;
            point = countPointRoad(justPlaced,TOP);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleRoute(justPlaced, i);
                give_point(list_meeple,list,point);
                removeMeepleRoadStart(justPlaced);
            }
            break;
    }
    switch (justPlaced->tile->left)
    {
        case VILLE:
            if(BLASON == middle || VILLE == middle)break;
            point = count_point_city(justPlaced,LEFT);
            printf("point =%d\n",point);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i+1,LEFT);
                give_point(list_meeple,list,point);
                removeMeepleVilleStart(justPlaced,LEFT);
            }
            break;
        
        case BLASON:
            if(BLASON == middle || VILLE == middle)break;
            point = count_point_city(justPlaced,LEFT);printf("point =%d\n",point);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i+1,LEFT);
                give_point(list_meeple,list,point);
                removeMeepleVilleStart(justPlaced,LEFT);
            }
            break;
        case ROUTE:
            if(justPlaced->tile->left == middle || middle == VILLAGE || middle == ABBAYES)break;
            point = countPointRoad(justPlaced,LEFT);            
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleRoute(justPlaced, i);
                give_point(list_meeple,list,point);
                removeMeepleRoadStart(justPlaced);
            }
            break;
    }
    switch (justPlaced->tile->bot)
    {
        case VILLE:
            if(BLASON == middle || VILLE == middle)break;
            point = count_point_city(justPlaced,BOT);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i+1,BOT);
                give_point(list_meeple,list,point);
                removeMeepleVilleStart(justPlaced,BOT);
            }
            break;
        
        case BLASON:
            if(BLASON == middle || VILLE == middle)break;
            point = count_point_city(justPlaced,BOT);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i+1,BOT);
                give_point(list_meeple,list,point);
                removeMeepleVilleStart(justPlaced,BOT);
            }
            break;

        case ROUTE:
            if(justPlaced->tile->bot == middle || middle == VILLAGE || middle == ABBAYES)break;
            point = countPointRoad(justPlaced,BOT);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleRoute(justPlaced, i);
                give_point(list_meeple,list,point);
                removeMeepleRoadStart(justPlaced);
            }
            break;
    }
}

void finDuJeu(struct Grid grille, struct list_player *list)
/*
Argument:
    Un pointeur vers la dernière tuile  posée.
    La liste des joueurs.
Retour:
    Void.
Description:
    La  fonction prend en paramètre la dernière tuile posée et compte les points que permet d'acquérir cette tuiles sur d'après la structure présente au milieu.
    (nottament si une abbaye est présente dans les alentours).
*/
{
    finJeu = 1;
    pointPlacedTile(temp,list);
}