#include <iostream>
#include "graph.h"

// constructor(s)
graph_c::graph_c()
{
  vertices = NULL;
}

graph_c::graph_c( vertex_t * starter )
{
  vertices = starter;
}

// destructor(s)
graph_c::~graph_c()
{
}

// getter(s) / setter(s)
vertex_t * graph_c::get_vertices()
{
  return( vertices );
}

void graph_c::set_vertices( vertex_t * vertices )
{
}

vertex_t * graph_c::get_vertex( graphElementT gelem )
{
  vertex_t * vertp;
  vertex_t * prevp;

  vertp = vertices;
  prevp = NULL;
  while( vertp != NULL ) {
    prevp = vertp;
    vertp = vertp->next;
    if( prevp->element == gelem )
      break;
  }

  return( prevp );
}

// methods

// uses the == operator 
int graph_c::add_vertex( vertex_t * added )
{
  vertex_t * pre_vert = NULL;
  vertex_t * cur_vert = vertices;

  while (cur_vert != NULL) {
    if( cur_vert->element == added->element ) {
      // vertex already there
      return -1;
    }
    pre_vert = cur_vert;
    cur_vert = cur_vert->next;
  }

  if (cur_vert == NULL) {
    vertices = added;
  }
  else {
    pre_vert->next = added;
  }

//  // TODO: This produced a warning (correctly):
//  // ‘pre_vert’ may be used uninitialized in this function
//  // I copied it above
//  if( vertices == NULL )
//    vertices = added;
//  else {
//    cur_vert = vertices;
//    while ( cur_vert != NULL ) {
//      if( cur_vert->element == added->element ) {
//        // vertex already there
//        return -1;
//      }
//      pre_vert = cur_vert;
//      cur_vert = cur_vert->next;
//    }
//  }
//
//  pre_vert->next = added;

  return 0;
}

int graph_c::remove_vertex( vertex_t * who )
{
  vertex_t * prev_vert;
  vertex_t * cur_vert;
  vertex_t * removed;

  if( vertices == NULL )
    std::cout << "no vertices to remove" << std::endl;
  else {
    // first remove all edges pointing to the to be removed vertex
    remove_all_edges_to( who );
    
    if(who == vertices) {
      removed = vertices;
      vertices = vertices->next;
      delete removed;
    }
    else {

      prev_vert = vertices;        
      cur_vert = prev_vert->next;

      while( cur_vert != NULL ) {
        if( who ==  cur_vert ) {
          removed = cur_vert;
          prev_vert->next = cur_vert->next;
          cur_vert->next = NULL;
          delete removed;
          break;
        }
        prev_vert = cur_vert;
        cur_vert = cur_vert->next;
      }
    }
  }

  return 0;
}

int graph_c::add_edge( vertex_t * from, vertex_t * to )
{
  edge_t * pre_edge;
  edge_t * cur_edge;
  edge_t * new_edge;

  new_edge = new edge_t;
  edge_t::init(new_edge, to);

  if( from->edges == NULL )
    from->edges = new_edge;
  else {
    cur_edge = from->edges;
    while( cur_edge != NULL ) {
      if( cur_edge->connectsTo->element == to->element ) {
        // edge already there so no need to add
        return -1;
      }
      pre_edge = cur_edge;
      cur_edge = cur_edge->next;
    }
    pre_edge->next = new_edge;
  }
  return 0;
}

int graph_c::remove_edge( vertex_t * from, vertex_t * to )
{
  edge_t * prev_edge;
  edge_t * cur_edge;  
  edge_t * removed;    
  
  if(from->edges == NULL)
    std::cout << "no edges to remove" << std::endl;
  else {  
    if(from->edges->connectsTo == to) {
      removed = from->edges;
      from->edges = from->edges->next;
      delete removed;
    }
    else {
      prev_edge = from->edges;        
      cur_edge = prev_edge->next;
      
      while( cur_edge != NULL ) {
        if( to ==  cur_edge->connectsTo ) {
          removed = cur_edge;
          prev_edge->next = cur_edge->next;
          cur_edge->next = NULL;
          delete removed;
          break;
        }
        prev_edge = cur_edge;
        cur_edge = cur_edge->next;
      }
    }
  }

  return 0;
}

int graph_c::remove_all_edges( vertex_t * whose )
{
  remove_all_edges_to( whose );
  remove_all_edges_from( whose );

  return 0;
}

int graph_c::remove_all_edges_from( vertex_t * from )
{
  for (edge_t * edgep = from->edges; edgep != NULL; edgep = edgep->next) {
    remove_edge( from, edgep->connectsTo );
  }
  return 0;
}

int graph_c::remove_all_edges_to( vertex_t * to )
{
  for(vertex_t * vertp = vertices; vertp != NULL; vertp = vertp->next ) {
    for (edge_t * edgep = vertp->edges; edgep != NULL; edgep = edgep->next) {
      if ( edgep->connectsTo == to )
        remove_edge( vertp, to );
    }
  }
  return 0;
}

unsigned int graph_c::count_vertices()
{
  unsigned int count;
  vertex_t * vertp;

  count = 0;
  vertp = vertices;
  while ( vertp != NULL ) {
    std::cout << "\'" << vertp->element << "\'" << std::endl;
    vertp = vertp->next;
    count++;
  }

  return( count );
}

unsigned int graph_c::count_edges( vertex_t * whose )
{
  unsigned int count;
  edge_t * cur_edge;

  count = 0;

  cur_edge = whose->edges;
  while( cur_edge != NULL ) {
    std::cout << "\t\'" << cur_edge->connectsTo->element << "\'" << std::endl;
    cur_edge = cur_edge->next;
    count++;
  }

  return( count );
}

int graph_c::detect_cycle()
{
  vertex_t * vertp;
  // 
  for( vertp = vertices; vertp != NULL; vertp = vertp->next ) {
    vertp->colour = WHITE;
  }
  
  for( vertp = vertices; vertp != NULL; vertp = vertp->next ) {
    if( vertp->colour == WHITE )
      if( coloured_dfs( vertp ) )
        return 1;
  }
  return 0;
}

int graph_c::coloured_dfs( vertex_t * vertp)
{
  edge_t * edgep;

  vertp->colour = GRAY;
  for ( edgep = vertp->edges; edgep != NULL; edgep = edgep->next ) {
    if( edgep->connectsTo->colour == GRAY ) {
      std::cout << "There's a cycle" << std::endl;
      return 1;
    }
    else if( edgep->connectsTo->colour == WHITE ) {
      if( coloured_dfs( edgep->connectsTo ) ) {
        return 1;
      }
    }
  } 
  vertp->colour = BLACK;

  return 0;
} 

void graph_c::printme()
{
  vertex_t * vertp;
  edge_t * edgep;

  for( vertp = vertices; vertp != NULL; vertp = vertp->next ) {
    std::cout << vertp->element << " ( ";
    for ( edgep = vertp->edges; edgep != NULL; edgep = edgep->next ) {
      std::cout << " " << edgep->connectsTo->element << " ";
    }
    std::cout << " )" << std::endl;
  }
}

static int reachable( graphElementT dest, vertex_t *startP )
{
  edge_t * edgeP;
  int reached = 0;

  // Have we been here already?
  if (startP->colour == BLACK)
    reached = 0;
  else if (startP->element == dest)   // Is this the destination?
    reached = 1;
  else {
    startP->colour = BLACK;  // Don't come here again.
    // See if we can get there from each of the vertices we connect to
    // If we can get there from at least one of them, it is reachable.
    for (edgeP = startP->edges; edgeP != NULL; edgeP = edgeP->next) {
      if (reachable(dest, edgeP->connectsTo))
        reached = 1;
    }
  }
  
  return( reached );
}



int GraphIsReachable( graphADT graph, graphElementT dest, graphElementT source )
{
  vertex_t *vertP;
  vertex_t *startP = NULL;

  // Go through each vertex
  for (vertP = graph->vertices; vertP != NULL; vertP = vertP->next) {
    // mark them as "not visited"...
    vertP->colour = WHITE;
    // ...and also record where the start vertex is
    if (vertP->element == source)
      startP = vertP;
  }

  // Make sure the starting point exists.
  if (startP == NULL)
    return 0;

  // Now see if we can get there from here.
  return reachable(dest, startP);
}
