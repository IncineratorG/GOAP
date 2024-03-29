#include "ExamplesRunner.h"

#include "WordState.h"
#include "Action.h"
#include "Planner.h"

#include <iostream>
#include <vector>

#include <QDebug>



ExamplesRunner* ExamplesRunner::self = nullptr;

ExamplesRunner* ExamplesRunner::getInstance() {
    if (!self)
        self = new ExamplesRunner();

    return self;
}

ExamplesRunner::ExamplesRunner() {

}

void ExamplesRunner::runMe() {
    qDebug() << __PRETTY_FUNCTION__;

    std::vector<goap::Action> actions;

    const int has_bread = 1;
    const int has_meat = 2;
    const int food_cooked = 3;
    const int hungry = 4;


    goap::Action goToShopForBread("Go for bread", 1);
    goToShopForBread.setPrecondition(has_bread, false);
    goToShopForBread.setEffect(has_bread, true);
    actions.push_back(goToShopForBread);

    goap::Action goToShopForMeat("Go for meat", 1);
    goToShopForMeat.setPrecondition(has_meat, false);
    goToShopForMeat.setEffect(has_meat, true);
    actions.push_back(goToShopForMeat);

    goap::Action cookFood("Cook", 200);
    cookFood.setPrecondition(has_meat, true);
    cookFood.setPrecondition(has_bread, true);
    cookFood.setEffect(food_cooked, true);
    actions.push_back(cookFood);

    goap::Action eat("Eat", 1);
    eat.setPrecondition(food_cooked, true);
    eat.setPrecondition(hungry, true);
    eat.setEffect(hungry, false);
    actions.push_back(eat);

    goap::Action orderFood("Order food", 500);
    orderFood.setPrecondition(food_cooked, false);
    orderFood.setPrecondition(hungry, true);
    orderFood.setEffect(food_cooked, true);
    actions.push_back(orderFood);


    goap::WorldState initialState;
    initialState.setVariable(has_bread, false);
    initialState.setVariable(has_meat, false);
    initialState.setVariable(food_cooked, false);
    initialState.setVariable(hungry, true);

    goap::WorldState goalState;
    goalState.setVariable(hungry, false);


    // Fire up the A* planner
    goap::Planner as;
    try {
        std::vector<goap::Action> the_plan = as.plan(initialState, goalState, actions);
        std::cout << "Found a path!\n";
        for (std::vector<goap::Action>::reverse_iterator rit = the_plan.rbegin(); rit != the_plan.rend(); ++rit) {
            std::cout << rit->name() << std::endl;
        }
    } catch (const std::exception&) {
        std::cout << "Sorry, could not find a path!\n";
    }
}


void ExamplesRunner::runWargame() {
    std::cout << "Wargame example running...\n";
    std::vector<goap::Action> actions;

    // Constants for the various states are helpful to keep us from
    // accidentally mistyping a state name.
    const int target_acquired = 10;
    const int target_lost = 15;
    const int target_in_warhead_range = 20;
    const int target_dead = 30;

    // Now establish all the possible actions for the action pool
    // In this example we're providing the AI some different FPS actions
    goap::Action spiral("searchSpiral", 5);
    spiral.setPrecondition(target_acquired, false);
    spiral.setPrecondition(target_lost, true);
    spiral.setEffect(target_acquired, true);
    actions.push_back(spiral);

    goap::Action serpentine("searchSerpentine", 5);
    serpentine.setPrecondition(target_acquired, false);
    serpentine.setPrecondition(target_lost, false);
    serpentine.setEffect(target_acquired, true);
    actions.push_back(serpentine);

    goap::Action intercept("interceptTarget", 5);
    intercept.setPrecondition(target_acquired, true);
    intercept.setPrecondition(target_dead, false);
    intercept.setEffect(target_in_warhead_range, true);
    actions.push_back(intercept);

    goap::Action detonateNearTarget("detonateNearTarget", 5);
    detonateNearTarget.setPrecondition(target_in_warhead_range, true);
    detonateNearTarget.setPrecondition(target_acquired, true);
    detonateNearTarget.setPrecondition(target_dead, false);
    detonateNearTarget.setEffect(target_dead, true);
    actions.push_back(detonateNearTarget);

    // Here's the initial state...
    goap::WorldState initial_state;
    initial_state.setVariable(target_acquired, false);
    initial_state.setVariable(target_lost, true);
    initial_state.setVariable(target_in_warhead_range, false);
    initial_state.setVariable(target_dead, false);

    // ...and the goal state
    goap::WorldState goal_target_dead;
    goal_target_dead.setVariable(target_dead, true);
    goal_target_dead.priority_ = 50;

    // Fire up the A* planner
    goap::Planner as;
    try {
        std::vector<goap::Action> the_plan = as.plan(initial_state, goal_target_dead, actions);
        std::cout << "Found a path!\n";
        for (std::vector<goap::Action>::reverse_iterator rit = the_plan.rbegin(); rit != the_plan.rend(); ++rit) {
            std::cout << rit->name() << std::endl;
        }
    }
    catch (const std::exception&) {
        std::cout << "Sorry, could not find a path!\n";
    }
}

void ExamplesRunner::runWeapon() {
    std::cout << "Weapon example running...\n";
    std::vector<goap::Action> actions;

    // Constants for the various states are helpful to keep us from
    // accidentally mistyping a state name.
    const int enemy_sighted = 1;
    const int enemy_dead = 2;
    const int enemy_in_range = 3;
    const int enemy_in_close_range = 4;
    const int inventory_knife = 5;
    const int inventory_gun = 6;
    const int gun_drawn = 7;;
    const int gun_loaded = 8;
    const int have_ammo = 9;
    const int knife_drawn = 10;
    const int weapon_in_hand = 11;
    const int me_dead = 12;

    // Now establish all the possible actions for the action pool
    // In this example we're providing the AI some different FPS actions
    goap::Action scout("scoutStealthily", 250);
    scout.setPrecondition(enemy_sighted, false);
    scout.setPrecondition(weapon_in_hand, true);
    scout.setEffect(enemy_sighted, true);
    actions.push_back(scout);

    goap::Action run("scoutRunning", 150);
    run.setPrecondition(enemy_sighted, false);
    run.setPrecondition(weapon_in_hand, true);
    run.setEffect(enemy_sighted, true);
    actions.push_back(run);

    goap::Action approach("closeToGunRange", 2);
    approach.setPrecondition(enemy_sighted, true);
    approach.setPrecondition(enemy_dead, false);
    approach.setPrecondition(enemy_in_range, false);
    approach.setPrecondition(gun_loaded, true);
    approach.setEffect(enemy_in_range, true);
    actions.push_back(approach);

    goap::Action approachClose("closeToKnifeRange", 4);
    approachClose.setPrecondition(enemy_sighted, true);
    approachClose.setPrecondition(enemy_dead, false);
    approachClose.setPrecondition(enemy_in_close_range, false);
    approachClose.setEffect(enemy_in_close_range, true);
    actions.push_back(approachClose);

    goap::Action load("loadGun", 2);
    load.setPrecondition(have_ammo, true);
    load.setPrecondition(gun_loaded, false);
    load.setPrecondition(gun_drawn, true);
    load.setEffect(gun_loaded, true);
    load.setEffect(have_ammo, false);
    actions.push_back(load);

    goap::Action draw("drawGun", 1);
    draw.setPrecondition(inventory_gun, true);
    draw.setPrecondition(weapon_in_hand, false);
    draw.setPrecondition(gun_drawn, false);
    draw.setEffect(gun_drawn, true);
    draw.setEffect(weapon_in_hand, true);
    actions.push_back(draw);

    goap::Action holster("holsterGun", 1);
    holster.setPrecondition(weapon_in_hand, true);
    holster.setPrecondition(gun_drawn, true);
    holster.setEffect(gun_drawn, false);
    holster.setEffect(weapon_in_hand, false);
    actions.push_back(holster);

    goap::Action drawKnife("drawKnife", 1);
    drawKnife.setPrecondition(inventory_knife, true);
    drawKnife.setPrecondition(weapon_in_hand, false);
    drawKnife.setPrecondition(knife_drawn, false);
    drawKnife.setEffect(knife_drawn, true);
    drawKnife.setEffect(weapon_in_hand, true);
    actions.push_back(drawKnife);

    goap::Action sheath("sheathKnife", 1);
    sheath.setPrecondition(weapon_in_hand, true);
    sheath.setPrecondition(knife_drawn, true);
    sheath.setEffect(knife_drawn, false);
    sheath.setEffect(weapon_in_hand, false);
    actions.push_back(sheath);

    goap::Action shoot("shootEnemy", 3);
    shoot.setPrecondition(enemy_sighted, true);
    shoot.setPrecondition(enemy_dead, false);
    shoot.setPrecondition(gun_drawn, true);
    shoot.setPrecondition(gun_loaded, true);
    shoot.setPrecondition(enemy_in_range, true);
    shoot.setEffect(enemy_dead, true);
    actions.push_back(shoot);

    goap::Action knife("knifeEnemy", 3);
    knife.setPrecondition(enemy_sighted, true);
    knife.setPrecondition(enemy_dead, false);
    knife.setPrecondition(knife_drawn, true);
    knife.setPrecondition(enemy_in_close_range, true);
    knife.setEffect(enemy_dead, true);
    actions.push_back(knife);

    goap::Action destruct("selfDestruct", 30);
    destruct.setPrecondition(enemy_sighted, true);
    destruct.setPrecondition(enemy_dead, false);
    destruct.setPrecondition(enemy_in_range, true);
    destruct.setEffect(enemy_dead, true);
    destruct.setEffect(me_dead, true);
    actions.push_back(destruct);

    // Now establish some goal states and an initial state
    goap::WorldState goal_win;
    goal_win.setVariable(enemy_dead, true);
    goal_win.setVariable(me_dead, false);
    goal_win.setVariable(weapon_in_hand, true);
    goal_win.priority_ = 100;

    // You can tweak these (e.g. have_ammo, the inventory items) to
    // elicit different plans from the AI.
    goap::WorldState initial_state;
    initial_state.setVariable(enemy_dead, false);
    initial_state.setVariable(enemy_sighted, false);
    initial_state.setVariable(enemy_in_range, false);
    initial_state.setVariable(enemy_in_close_range, false);
    initial_state.setVariable(gun_loaded, false);
    initial_state.setVariable(gun_drawn, false);
    initial_state.setVariable(knife_drawn, false);
    initial_state.setVariable(weapon_in_hand, false);
    initial_state.setVariable(me_dead, false);
    initial_state.setVariable(have_ammo, true);
    initial_state.setVariable(inventory_knife, true);
    initial_state.setVariable(inventory_gun, true);

    // Fire up the A* planner
    goap::Planner as;
    try {
        std::vector<goap::Action> the_plan = as.plan(initial_state, goal_win, actions);
        std::cout << "Found a path!\n";
        for (std::vector<goap::Action>::reverse_iterator rit = the_plan.rbegin(); rit != the_plan.rend(); ++rit) {
            std::cout << rit->name() << std::endl;
        }
    } catch (const std::exception&) {
        std::cout << "Sorry, could not find a path!\n";
    }
}
