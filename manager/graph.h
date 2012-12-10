#ifndef GRAPH_H_
#define GRAPH_H_

#define WHITE 0
#define BLACK 1
#define GRAY 2

typedef unsigned long int graphElementT;

typedef struct graphCDT * graphADT;

typedef struct vertexTag {
  graphElementT element;
  int colour;
  struct edgeTag * edges;
  struct vertexTag * next;
} vertex_t;

typedef struct edgeTag {
  struct vertexTag * connectsTo;
  struct edgeTag * next;

  static void init( edgeTag * edg, vertexTag * towhere )
  {
    edg->connectsTo = towhere;
    edg->next = NULL;
  }
} edge_t;

typedef struct graphCDT {
  vertex_t * vertices;
} graphCDT;

class graph_c {
  private:
    vertex_t * vertices;
  protected:
  public:
    // constructor(s)
    graph_c();
    graph_c( vertex_t * );
    // destructor
    ~graph_c();
    // getter(s) / setter(s)
    vertex_t * get_vertices();
    void set_vertices( vertex_t * );
    vertex_t * get_vertex( graphElementT );
    // methods
    int add_vertex( vertex_t * );
    int remove_vertex( vertex_t * );
    int add_edge( vertex_t *, vertex_t * );
    int remove_edge( vertex_t *, vertex_t * );
    int remove_all_edges( vertex_t * );
    int remove_all_edges_from( vertex_t * );
    int remove_all_edges_to( vertex_t * to );
    unsigned int count_vertices();
    unsigned int count_edges( vertex_t * );

    int detect_cycle();
    int coloured_dfs( vertex_t * );
    
    void printme();
};

#endif /*GRAPH_H_*/
