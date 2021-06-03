#pragma once

#include <random>

#include "Mesh/Vertex.hpp"
#include <cmath>

#include <iostream>

class Generator {

public:
	// 
	// terrain: Initial configuration of the terrain to divide
	// 
	//    Before Division       |   After Division
    // -------------------------+----------------------------
	//                          |
	// 	   A +-------------+ B  |     A +------ G -------+ B
	// 	     | \	  	   |    |       | \  	| \      |
	// 	     |   \         |    |       |     \ | E    \ |
	// 	     | 	    \      |    |     F +-------+--------+ H
	//       | 	       \   |    |       | \     | \      | 
	// 	     | 	         \ |    |       |     \ |      \ | 
	// 	   C +-------------+ D  |     C +------ I -------+ D
	// 
	static inline Vertex* Generate(Vertex* terrain, int iterations, float d, int r)
	{
		if (terrain == nullptr)
			return nullptr;

		// The Terrain is divided left to right, up to down
		Vertex* A, * B, * C, * D = nullptr;
		Vertex* E, * F, * G, * H, * I = nullptr;

		auto head = terrain;
		auto line = head;

		for(int i = 0 ; i < iterations ; i++)
		{
			line = head;
			auto next = line->down;

			while (line->down != nullptr)
			{
				A = line;

				while (A->right != nullptr)
				{
					D = A->diagonal;
					B = D->up;
					C = D->left;

					// 1 - Criar ponto central
					E = CalculateVertex(A, B, C, D, random(d));

					// 2 - Criar as bordas do
					if (D->left->id == A->down->id)
						F = CalculateVertex(A, C, E, random(d));
					else
						F = CompleteVertex(A->down, A, A->down->down, A->down->left, E, random(d));

					if (D->up->id == A->right->id)
						G = CalculateVertex(A, B, E, random(d));
					else
					    G = CompleteVertex(A->right, A, A->right->right, A->right->up, E, random(d));

					 H = CalculateVertex(B, D, E, random(d));
					 I = CalculateVertex(C, D, E, random(d));

					// 3 - Realizar ligações dos novos pontos
					// A
					A->right = G;
					A->down  = F;
					A->diagonal = E;
			    
					// G
					G->right = B;
					G->down  = E;
					G->left  = A;
					G->diagonal = H;

					// B
					B->down = H;
					B->left = G;

					// F
					F->up    = A;
					F->right = E;
					F->down  = C;
					F->diagonal = I;

					// E
					E->up    = G;
					E->right = H;
					E->down  = I;
					E->left  = F;
					E->diagonal = D;

					// H
					H->up   = B;
					H->down = D;
					H->left = E;

					// C
					C->up    = F;
					C->right = I;

					// I
					I->up    = E;
					I->right = D;
					I->left  = C;

					// C
					D->up   = H;
					D->left = I;

					A = A->right->right;
				}				

			    // 4 - Update d
				d = d * ((float) pow(2, -r));

				line = line->down->down;
			}
		}
		int a = 1;
		return terrain;
	}

	// Four Neightbours
	static inline Vertex* CalculateVertex(Vertex* A, Vertex* B, Vertex* C, Vertex* D, float r)
	{
		// E = (A + B + C + D) / 4 + Rand(d)
		float x = (C->point.x + A->point.x) / 2.0f;
		float z = (B->point.z + A->point.z) / 2.0f;
		float y = ((D->point.y + C->point.y + B->point.y + A->point.y) / 4.0f)+r;

		return new Vertex(x, y, z);
	}

	// Three Neightbours
	inline static Vertex* CalculateVertex(Vertex* side1, Vertex* side2, Vertex* center, float r)
	{
		// P = (A + B + C) / 4 + Rand(d)

		float x = (side2->point.x + side1->point.x) / 2.0f;
		float z = (side2->point.z + side1->point.z) / 2.0f;
		float y = ((center->point.y + side2->point.y + side1->point.y) / 3.0f) +r;

		return new Vertex(x, y, z);
	}

	// Three to Four Neightbours
	inline static Vertex* CompleteVertex(Vertex* P, Vertex* A, Vertex* B, Vertex* C, Vertex* D, float r)
	{
		// P = (A + B + C + D) / 4 + Rand(d)
		P->SetY(((D->point.y + C->point.y + B->point.y + A->point.y) / 4.0f)+r);

		return P;
	}


	inline static float random(int range)
	{
		int d = range < 1 ? 1 : range;

		return (rand() % (d * 2)) - d;
	}
};
