/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef BODY_H_
#define BODY_H_

#include "..\math\xform.h"

class CollisionShape;

class Body
{
public:
   Body();
   ~Body();

   const Vector& getPosition() const;
   void          setPosition(const Vector& position);

   float getMass() const;
   void  setMass(float mass);

   float getInertia() const;
   void  setInertia(float inertia);

 // shapes
   void  addShape(CollisionShape* pshape);

 // forces
   void  addForce(const Vector& force);
   void  addForce(const Vector& force, const Vector& location);
   void  addWorldForce(const Vector& force, const Vector& location);
   void  addTorque(float torque);

   void  integrate(float timestep);

private:
   Vector localToWorld(const Vector& vector) const;

   void calculateDerivedData();
   void clearAccumulates();

   XForm    mTransform;
   Vector   mPosition;
   float    mAngle;
   
   Vector   mLinearVelocity;
   float    mAngularVelocity;

   Vector   mAccumForce;
   float    mAccumTorque;

   float    mLinearDamping;
   float    mAngularDamping;

   float    mInverseInertia;
   float    mInverseMass;

   CollisionShape*  mpShape;
};

#ifdef JENGINE_INLINE
#  include "body.inl"
#endif

#endif
