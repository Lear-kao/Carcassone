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
{
    char point = 0;
    char unfinished=0;
    
    
    switch (a)
    {
        case RIGHT:
            //grille->marquer = v_marquer;
            if(grille->right != NULL)
            {
                point += isFinishedCity(grille->right, &unfinished);
            }
            else unfinished = 1;
            break;
        
        case TOP:
            //grille->marquer = v_marquer;
            if(grille->top != NULL)
            {
                point += isFinishedCity(grille->top, &unfinished);
            }
            else unfinished = 1;
            break;
    
        case LEFT:
            //grille->marquer = v_marquer;
            if(grille->left != NULL)
            {
                point += isFinishedCity(grille->left, &unfinished);
            }
            else unfinished = 1;
            break;
        
        case BOT:
            //grille->marquer = v_marquer;
            if(grille->bot != NULL)
            {
                point += isFinishedCity(grille->bot, &unfinished);
            }
            else unfinished = 1;
            break;
        
        case MIDDLE:
            point = isFinishedCity(grille,&unfinished);
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
                if(grille->top->tile->bot==VILLE)
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
                if(grille->right->tile->left==VILLE)
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
                if(grille->left->tile->right==VILLE)
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
                if(grille->bot->tile->top==VILLE)
                cmpt += isFinishedCity(grille->bot,unfinished);
            }
        }
    }
    if (*unfinished == 1 && finJeu == 0) return 0;
    return cmpt;
}

char countPointRoad(struct Grid *grille,char *unfinished, enum places start)
{
    char  point = 0;
    switch(start)
    {
        case RIGHT:
            if(grille->right == NULL  || is_a_potential_tile(grille->right->tile))
            {
                *unfinished = 1;
                break;
            }
            point += isFinishedRoad(grille->right,unfinished);
            break;
        case TOP:
            if(grille->top == NULL  || is_a_potential_tile(grille->top->tile))
            {
                *unfinished = 1;
                break;
            }
            point += isFinishedRoad(grille->top,unfinished);
            break;
        case LEFT:
            if(grille->left == NULL  || is_a_potential_tile(grille->left->tile))
            {
                *unfinished = 1;
                break;
            }
            point += isFinishedRoad(grille->left,unfinished);
            break;
        case BOT:
            if(grille->bot == NULL  || is_a_potential_tile(grille->bot->tile))
            {
                *unfinished = 1;
                break;
            }
            point += isFinishedRoad(grille->bot,unfinished);
            break;
    }

    v_marquer++;
    if (*unfinished == 1 && finJeu == 0)
    {
        return 0;
    }
    return point;
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
    if (grille->tile->middle == ROUTE){
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
    }
    if(grille->tile->middle == VILLAGE || grille->tile->middle == VILLE || grille->tile->middle == ABBAYES)
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
    char list_meeple[nbPlayers];
    switch (justPlaced->tile->middle)
    {
        case VILLE:
            point = count_point_city(justPlaced,4);
            for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i);
            give_point(list_meeple,listPlayer,point);
            break;
        case ABBAYES:
            point = count_point_city(justPlaced,4);
            for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleAbbaye(justPlaced, i);
            give_point(list_meeple,listPlayer,point);
            break;
        case ROUTE:
            break;
    }
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
        if(list_meeple_player[i] == max(list_meeple_player)) list->player[i]->points += point; 
    }
    
}

void secondary_verification(struct Grid *justPlaced, struct list_player *list, enum types middle)
{
    char point;
    char list_meeple[nbPlayers];
    char unfinished_road = 1;
    switch (justPlaced->tile->right)
    {
        case VILLE:
            if(justPlaced->tile->right == middle)break;
            point = count_point_city(justPlaced,RIGHT);
            for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i);
            give_point(list_meeple,list,point);
            break;
        case ROUTE:
            if(justPlaced->tile->right == middle || middle ==VILLAGE || middle == ABBAYES)break;
            point = countPointRoad(justPlaced,&unfinished_road,RIGHT);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                //for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleRoute(justPlaced, i);
                give_point(list_meeple,list,point);
            }
            break;
    }
    switch (justPlaced->tile->top)
    {
        case VILLE:
            if(justPlaced->tile->top == middle)break;
            point = count_point_city(justPlaced,TOP);
            for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i);
            give_point(list_meeple,list,point);
            break;
        case ROUTE:
            if(justPlaced->tile->top == middle || middle == VILLAGE || middle == ABBAYES)break;
            point = countPointRoad(justPlaced,&unfinished_road,TOP);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                //for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleRoute(justPlaced, i);
                give_point(list_meeple,list,point);
            }
            break;
    }
    switch (justPlaced->tile->left)
    {
        case VILLE:
            if(justPlaced->tile->left == middle)break;
            point = count_point_city(justPlaced,LEFT);
            for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i);
            give_point(list_meeple,list,point);
            break;
        case ROUTE:
            if(justPlaced->tile->left == middle || middle == VILLAGE || middle == ABBAYES)break;
            point = countPointRoad(justPlaced,&unfinished_road,LEFT);            
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                //for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleRoute(justPlaced, i);
                give_point(list_meeple,list,point);
            }
            break;
    }
    switch (justPlaced->tile->bot)
    {
        case VILLE:
            if(justPlaced->tile->bot == middle)break;
            point = count_point_city(justPlaced,BOT);
            for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleVille(justPlaced, i);
            give_point(list_meeple,list,point);
            break;
        case ROUTE:
            if(justPlaced->tile->bot == middle || middle == VILLAGE || middle == ABBAYES)break;
            point = countPointRoad(justPlaced,&unfinished_road,BOT);
            if(point != 0)
            {
                char list_meeple[nbPlayers];
                //for (int i = 0; i < nbPlayers; i++) list_meeple[i] = nbMeepleRoute(justPlaced, i);
                give_point(list_meeple,list,point);
            }
            break;
    }
}

void finDuJeu(struct Grid grille, struct list_player *list)
{
    finJeu = 1;
    /* créer une fonction pour parcourir toute la grille et récupérer tout les points restants */
}