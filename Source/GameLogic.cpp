#include "GameLogic.h"
#include <iostream>

Vector2 PlayerMovementInput()
{ //Retrieves the player input in the cardinal directions. Returns a vector between {-1,-1} and {1,1}.
	Vector2 movementInput = { (float)(IsKeyDown(KEY_D) - (float)IsKeyDown(KEY_A)),
		(float)(IsKeyDown(KEY_S) - (float)IsKeyDown(KEY_W)) };
	Vector2 movementOuput = (movementInput);
	return movementOuput;
}

Vector2 FuturePosition(MovingEntity* movingEntity)
{//Used to pass a future position into an argument, instead of current one. Prevents tunneling before calculating new collision.
	return (movingEntity->position + movingEntity->velocity);
}

void UpdateEntityVelocity(MovingEntity* e, Vector2 vec)
{// Moves an entity in the direction of it's vector interpolating it to its current velocity (to make it accelerate, and decelerate)
	Vector2 movement = { vec.x * MAXSMOVESPEED, vec.y * MAXSMOVESPEED };
	if (Vector2Length(movement) != 0)
	{
		e->velocity = Vector2Lerp(e->velocity, movement, LERP_T);
	}
	else
	{
		e->velocity = Vector2Lerp(e->velocity, movement, LERP_T * 3);
	}
}

Vector2 UpdateBombDropSite(MovingEntity* entity)
{ // Returns the point where the bomb should be dropped that is a cardinal direction away from the player.
	Vector2 bombDropPosition = {};
	//Work out the normal of the vector of player movement. The magnitude of the vector shouldn't be 0, which is checked before the function is called.
	Vector2 dropSiteNormal = (Vector2Normalize(entity->velocity));
	//We then determine which cardinal direction the player's current vector points towards, and place the bomb drop point there.
	if (dropSiteNormal.x >= 0)
	{
		if (dropSiteNormal.x >= abs(dropSiteNormal.y))
		{
			bombDropPosition = entity->position + Vector2{ BOMB_DROPDISTANCE, 0 };
		}
		else
		{
			if (dropSiteNormal.y >= 0)
			{
				bombDropPosition = entity->position + Vector2{ 0, BOMB_DROPDISTANCE };
			}
			else
			{
				bombDropPosition = entity->position - Vector2{ 0, BOMB_DROPDISTANCE };
			}
		}
	}
	else
	{
		if (abs(dropSiteNormal.x) >= abs(dropSiteNormal.y))
		{
			bombDropPosition = entity->position - Vector2{ BOMB_DROPDISTANCE, 0 };
		}
		else
		{
			if (dropSiteNormal.y >= 0)
			{
				bombDropPosition = entity->position + Vector2{ 0, BOMB_DROPDISTANCE };
			}
			else
			{
				bombDropPosition = entity->position - Vector2{ 0, BOMB_DROPDISTANCE };
			}
		}
	}
	return bombDropPosition;
}

//-----COLLISION LOGIC-----//
//-----COLLISION CHECKS
// These methods check if the entities/shapes are colliding, if so; returns 'true' then move to COLLISION LOGIC. If it ain't colliding, don't do the math! 
float CheckClosestPoint(Vector2 leftPoint, Vector2 rightPoint)
{
	//Return distance using pythagoran theorem to create a hypoteneuse. Returns the length which can be compared to another hypoteneuse, or length in general.
	float dx = leftPoint.x - rightPoint.x;
	float dy = leftPoint.y - rightPoint.y;
	float hypoteneuse = sqrtf(dx * dx + dy * dy);
	return hypoteneuse;
}

bool CheckPointInsideRectangle(Vector2 point, StaticEntity* staticEntity, Vector2 offsetRange)
{
	//Returns true if point is inside the rectangle area of staticEntity. Determined through Separating Axis Theorem (SAT). Rectangle can be offset, if necessary (used in Circle vs AABB).
	Vector2 staticEntityCalculationPosition = staticEntity->position - offsetRange;
	Vector2 staticEntityBottomRightCorner = staticEntityCalculationPosition + Vector2{ staticEntity->size,staticEntity->size } + offsetRange * 2;

	//checks if Vector2 point is between the two x positions of the rectangles area
	if (point.x > staticEntityCalculationPosition.x && point.x < staticEntityBottomRightCorner.x)
	{
		//if Vector2 point is between the x positions of the rectangle's area, then check y as well.
		if (point.y > staticEntityCalculationPosition.y && point.y < staticEntityBottomRightCorner.y)
		{
			//if true, then the point is inside the area of the box.
			return true;
		}
		else return false;
	}
	else return false;
}

bool CheckCircleVsCircle(MovingEntity* leftEntity, MovingEntity* rightEnity)
{
	//Can use pythagoras to check the hypoteneuse to the circles. If hypoteneuse is smaller than both radii then they are colliding
	if (CheckClosestPoint(FuturePosition(leftEntity), rightEnity->position) < leftEntity->size + rightEnity->size)
	{
		return true;
	}
	else return false;
}

bool CheckCircleVsAABB(MovingEntity* movingEnity, StaticEntity* staticEntity)
{
	//This script imagines the AABB as two separate AABBs that their respective x- and y- axis origins extended by the radius of the circle, and then imagines 4 circles at each corner of the original AABB. This will give us an enlarged rounded rectangle.
	//Start by checking we check for intersection with the x-axis- or y-axis-extended AABBs
	if (CheckPointInsideRectangle(FuturePosition(movingEnity), staticEntity, Vector2{ movingEnity->size,0 }) ||
		CheckPointInsideRectangle(FuturePosition(movingEnity), staticEntity, Vector2{ 0, movingEnity->size }))
	{
		return true;
	}
	//if none of those return true, we check if we "intersect" with the imaginary circles at any of the AABBs corners
	if (CheckClosestPoint(FuturePosition(movingEnity), staticEntity->position) < movingEnity->size ||
		CheckClosestPoint(FuturePosition(movingEnity), staticEntity->position + Vector2{ staticEntity->size,0 }) < movingEnity->size ||
		CheckClosestPoint(FuturePosition(movingEnity), staticEntity->position + Vector2{ 0, staticEntity->size }) < movingEnity->size ||
		CheckClosestPoint(FuturePosition(movingEnity), staticEntity->position + Vector2{ staticEntity->size, staticEntity->size }) < movingEnity->size)
	{
		return true;
	}
	return false;
}

bool CheckCircleVsLineSegment(MovingEntity* movingEnity, Vector2 lineSegmentStart, Vector2 lineSegmentEnd)
{
	//Retrieve position of the centre of Circle (C), as well as it's radius.
	Vector2 circlePosition = FuturePosition(movingEnity);
	float circleRadius = movingEnity->size;
	//First: Calculate the line Segment that is the vector from a start point (A) to an end point (B)
	Vector2 AB = { lineSegmentEnd - lineSegmentStart };
	//Second: vector AC is the vector from A and tC (Center of Circle after movement) calculated by subtracting vector A from vector tC
	Vector2 AtC = { circlePosition - lineSegmentStart };
	//Third: Project vector AC onto Linesegment AB to find the value of t; where along AB collision would occur
	float t = (Vector2DotProduct(AtC, AB) / Vector2DotProduct(AB, AB));
	if (t < 0 || t > 1)
	{
		//if t isn't between 0 or 1 then the circle is outside both ends of the linesegment.
		return false;
	}
	//Fourth: Vector P is where from A along AB the circle will intersect, which is a product of t.
	Vector2 P = (lineSegmentStart + (AB * t));
	//Fifth, vector CP is the vector from tempPosition to vector P calculated by subtracting tempPosition from P
	Vector2 CP = (P - circlePosition);
	//Sixth: We can now check if there is a collision by comparing the magnitude of vector CP to the radius of the circle. 
	if (Vector2Length(CP) < movingEnity->size)
	{
		//if the vector CP is smaller than the size of the circle Radii, then they are intersecting, otherwise they are not.
		return true;
	}
	else return false;
}

bool CheckAABBvsAABB(StaticEntity* leftEntity, StaticEntity* rightEntity)
{
	//I will use Projection onto the cardinal axii. If X has some overlap, then check Y too if X overlap. If both overlap = collision.
	Vector2 leftEntityBottomRight = { leftEntity->position.x + leftEntity->size, leftEntity->position.y + leftEntity->size };
	Vector2 rightEntityBottomRight = { rightEntity->position.x + rightEntity->size, rightEntity->position.y + rightEntity->size };

	//Now check if the minimum x coordinate of either AABB's x position is greater than the other AABB's maximum x. If it is, then they are not overlapping.
	//the LowerBottom represent the higher x and y value in both instances.
	if (leftEntity->position.x >= rightEntityBottomRight.x || rightEntity->position.x >= leftEntityBottomRight.x)
	{
		return false;
	}
	//if there is any overlap here, then we check y value as well.
	if (leftEntity->position.y >= rightEntityBottomRight.y || rightEntity->position.y >= leftEntityBottomRight.y)
	{
		return false;
	}
	//if neither check returns a false, then they are overlapping on both axii and there is collision!
	else return true;
}

//-----COLLISION RESOLUTION
// These scripts run when we've established that objects WILL collide.
void CircleCircleCollision(MovingEntity* leftEntity, MovingEntity* rightEntity)
{
	//This collision event will make the leftEntity "kick" the rightEntity by adding the product of leftEntity' velocity to the rightEntity.
	rightEntity->velocity = leftEntity->velocity * BOMB_KICKFORCE;
}

//Circle vs Axis-aligned bounding box
void CircleAABBCollision(MovingEntity* movingEntity, StaticEntity* staticEntity)
{
	//Checks creates a collision resolution based on what kind of entity movingEntity is. Creates a line segment usingthe two corners that are the closest to the player.
	if (CheckClosestPoint(FuturePosition(movingEntity), staticEntity->position) < /*If the movingEntity is closer to the top-left corner...*/
		CheckClosestPoint(FuturePosition(movingEntity), staticEntity->position + Vector2{ staticEntity->size, staticEntity->size })) /*... than the bottom-right corner*/
	{
		/*Find the second closest corner of the AABB aside from the top-left..*/
		if (CheckClosestPoint(FuturePosition(movingEntity), staticEntity->position + Vector2{ staticEntity->size, 0 }) < /*If top-right corner...*/
			CheckClosestPoint(FuturePosition(movingEntity), staticEntity->position + Vector2{ 0, staticEntity->size })) /*... is closer than bottom-left corner to top-left corner (origin)*/
		{
			//We can now generate a collision outcome for the movingEntity using the line segment from Top-left to Top-Right. 
			if (movingEntity->kind == PLAYER)
			{
				movingEntity->velocity.y = 0.f;
			}
			if (movingEntity->kind == ENEMY)
			{
				movingEntity->velocity = movingEntity->velocity * -1;
			}
			if (movingEntity->kind == BOMB)
			{
				if (staticEntity->destructible)
				{
					movingEntity->isAlive = false;
				}
				movingEntity->velocity = GenerateBounceVelocity(movingEntity, staticEntity->position, staticEntity->position + Vector2{ staticEntity->size, 0 });
			}
		}
		else /*... otherwise if the bottom-left is closer to top-left*/
		{
			//We can now generate a collision outcome for the movingEntity using the line segment from Top-left to Bottom-Left
			if (movingEntity->kind == PLAYER)
			{
				movingEntity->velocity.x = 0.f;
			}
			if (movingEntity->kind == ENEMY)
			{
				movingEntity->velocity = movingEntity->velocity * -1;
			}
			if (movingEntity->kind == BOMB)
			{
				if (staticEntity->destructible)
				{
					movingEntity->isAlive = false;
				}
				movingEntity->velocity = GenerateBounceVelocity(movingEntity, staticEntity->position, staticEntity->position + Vector2{ 0, staticEntity->size });
			}
		}
	}
	else /*If else, then the bottom-right was closest, so now find the second closes corner of the AABB aside from the bottom-right*/
	{
		/*Find the second closest corner of the AABB aside from the bottom-right..*/
		if (CheckClosestPoint(FuturePosition(movingEntity), staticEntity->position + Vector2{ staticEntity->size, 0 }) < /*If top-right corner...*/
			CheckClosestPoint(FuturePosition(movingEntity), staticEntity->position + Vector2{ 0, staticEntity->size })) /*... is closer than bottom-left corner to bottom-right corner*/
		{
			//We can now generate a collision outcome for the movingEntity using the line segment from Bottom-Right to Top-Right. 
			if (movingEntity->kind == PLAYER)
			{
				movingEntity->velocity.x = 0.f;
			}
			if (movingEntity->kind == ENEMY)
			{
				movingEntity->velocity = movingEntity->velocity * -1;
			}
			if (movingEntity->kind == BOMB)
			{
				if (staticEntity->destructible)
				{
					movingEntity->isAlive = false;
				}
				movingEntity->velocity = GenerateBounceVelocity(movingEntity, staticEntity->position + Vector2{ staticEntity->size, staticEntity->size },
					staticEntity->position + Vector2{ staticEntity->size, 0 });
			}
		}
		else /*... otherwise if the bottom-left is closer to bottom-right*/
		{
			//We can now generate a collision outcome for the movingEntity using the line segment from Bottom-Right to Bottom-Left
			if (movingEntity->kind == PLAYER)
			{
				movingEntity->velocity.y = 0.f;
			}
			if (movingEntity->kind == ENEMY)
			{
				movingEntity->velocity = movingEntity->velocity * -1;
			}
			if (movingEntity->kind == BOMB)
			{
				if (staticEntity->destructible)
				{
					movingEntity->isAlive = false;
				}
				movingEntity->velocity = GenerateBounceVelocity(movingEntity, staticEntity->position + Vector2{ staticEntity->size, staticEntity->size },
					staticEntity->position + Vector2{ 0, staticEntity->size });
			}
		}
	}

	/*If collision is with a Blast*/
	if (movingEntity->kind == BLAST && staticEntity->kind == BOX)
	{//Kills both the Blast and the Box
		movingEntity->isAlive = false;
		staticEntity->isAlive = false;
	}
	else if (movingEntity->kind == BLAST && staticEntity->kind == WALL)
	{//Only kills the Blast
		movingEntity->isAlive = false;
	}
}

//Circle vs Line segment
void CircleLineSegmentCollision(MovingEntity* movingEntity, Vector2 lineSegmentStart, Vector2 lineSegmentEnd)
{
	//First, establish the normal of the line segment the movingEntity is colliding with.
	//Line segment in this case is the Arena Border.
	Vector2 lineSegmentNormal = Vector2((lineSegmentEnd.y - lineSegmentStart.y), (lineSegmentEnd.x - lineSegmentStart.x));
	Vector2 unitNormal = lineSegmentNormal / (Vector2Length(lineSegmentNormal));

	if (movingEntity->kind == PLAYER)
	{
		//Player will just stop when up against a line segment
		if (abs(unitNormal.x) > 0)
		{ //If it is the normal of a vertical line (left, or right arena border), then cancel the Player's velocity.x
			movingEntity->velocity.x = 0.f;
		}
		if (abs(unitNormal.y) > 0)
		{ //If this is the normal of a horizontal line (top, or bottom arena border), then cancel the Player's velocity.y
			movingEntity->velocity.y = 0.f;
		}
	}
	if (movingEntity->kind == ENEMY)
	{
		//Player will just stop when up against a line segment
		if (abs(unitNormal.x) > 0)
		{ //If it is the normal of a vertical line (left, or right arena border), then cancel the Player's velocity.x
			movingEntity->velocity = movingEntity->velocity * -1.f;
		}
		if (abs(unitNormal.y) > 0)
		{ //If this is the normal of a horizontal line (top, or bottom arena border), then cancel the Player's velocity.y
			movingEntity->velocity = movingEntity->velocity * -1.f;
		}
	}
	if (movingEntity->kind == BOMB)
	{
		//Bombs should bounce off a line segment
		movingEntity->velocity = GenerateBounceVelocity(movingEntity, lineSegmentStart + (unitNormal * movingEntity->size),
			lineSegmentEnd + (unitNormal * movingEntity->size));
	}
	if (movingEntity->kind == BLAST)
	{
		//Blasts should be destroyed by a line segment.
		movingEntity->isAlive = false;
	}
}

Vector2 GenerateBounceVelocity(MovingEntity* movingEnity, Vector2 lineSegmentStart, Vector2 lineSegmentEnd)
{ //Will generate a bounce vector after a collision is detected.
	Vector2 lineSegmentNormal = Vector2((lineSegmentEnd.y - lineSegmentStart.y), (lineSegmentEnd.x - lineSegmentStart.x));
	Vector2 unitNormal = lineSegmentNormal / (Vector2Length(lineSegmentNormal));

	//Now we generate the new velocity vector; the "bounce"
	Vector2 projectionToUnitNormal = unitNormal * (2 * (Vector2DotProduct(movingEnity->velocity, unitNormal)));
	Vector2 bounceVelocity = movingEnity->velocity - projectionToUnitNormal;

	//Set the new vector velocity with a set bonus speed.
	Vector2 unitVectorBounceVelocity = (bounceVelocity / Vector2Length(bounceVelocity));
	bounceVelocity = unitVectorBounceVelocity * VELOCITYMULTIPLIER;

	//If the bounce velocity is too small, then add additional bounce by multiplying again.
	while (Vector2Length(bounceVelocity) <= MIN_BOUNCEVELOCITY)
	{
		bounceVelocity* VELOCITYMULTIPLIER;
	}
	return bounceVelocity;
}