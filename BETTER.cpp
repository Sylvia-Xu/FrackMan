//
//  BETTER.cpp
//  FrackMan
//
//  Created by 徐子悦 on 16/2/24.
//  Copyright © 2016年 David Smallberg. All rights reserved.
//

#include <stdio.h>

//make your code extendible
void canPlayerPickUpItem(Actor *x)
{
    if (x->canPlayerPickUp()) {
        pickUpItem(x);
    }
}

//who should activate the pick up function? the treasure itself or frackman?
//it is up to you. 