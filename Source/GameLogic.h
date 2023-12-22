#pragma once
#include "Gamestate.h"

//-----Game Logic Header File-----//
//Game logic dictates the behaviour of collision, movement, and entities during playtime.


//----MOVEMENT LOGIC-----//

Vector2 PlayerMovementInput(); //Returns a unit vector for which direction the player is moving based on input.

Vector2 FuturePosition(MovingEntity* movingEntity); //Pass the future position, assuming a frame of uninterrupted movement by the movingEntity's velocity.

void UpdateEntityVelocity(MovingEntity* e, Vector2 vec); //Moves player using a unit vector

Vector2 UpdateBombDropSite(MovingEntity* entity);

//-----COLLISION LOGIC-----//
//-----COLLISION CHECKS
float CheckClosestPoint(Vector2 leftPoint, Vector2 rightPoint); //Returns the distance bewteen two points as an hypoteneuse. Can be used for many different things.

bool CheckPointInsideRectangle(Vector2 point, StaticEntity* staticEntity, Vector2 offsetRange); //Checks if a point is inside the rectangle area of a staticEntity. Takes an overload for offset from position, if needed. Otherwise set to {0,0}.

bool CheckCircleVsCircle(MovingEntity* leftEntity, MovingEntity* rightEnity); // Usually Player vs Bomb, or enemy

bool CheckCircleVsAABB(MovingEntity* movingEnity, StaticEntity* staticEntity); // Usually Blast vs Box

bool CheckCircleVsLineSegment(MovingEntity* movingEnity, Vector2 lineSegmentStart, Vector2 lineSegmentEnd); // Usually Player vs Arena Boundaries

bool CheckAABBvsAABB(StaticEntity* leftEntity, StaticEntity* rightEntity); // Only used in test-mode.

//-----COLLISION RESOLUTION
//Circle vs Circle
void CircleCircleCollision(MovingEntity* leftEntity, MovingEntity* rightEntity); //Should launch rightEntity away by applying leftEnitity's velocity to rightEntity.

//Circle vs Axis-aligned bounding box
void CircleAABBCollision(MovingEntity* movingEntity, StaticEntity* staticEntity); //Creates a line segment on the staticEntity's AABB by locating its two closest corners to the movingEntity, and generating a bounceCollisionVector from that line segment.
//if the movingEnitity is a Blast for a Bomb, and staticEntity is a Box, then both should die and be deleted.

//Circle vs Line segment
void CircleLineSegmentCollision(MovingEntity* EntityPosition, Vector2 LineSegmentStart, Vector2 LineSegmentEnd); //Automatically sweeptests to see if thet next frame would result in a collision, and if so produce a response


Vector2 GenerateBounceVelocity(MovingEntity* movingEnity, Vector2 lineSegmentStart, Vector2 lineSegmentEnd); //Generates a bounce velocity by creating a normal of the resulting vector of the start and end point.