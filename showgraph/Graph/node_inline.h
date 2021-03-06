/**
 * @file: node_inline.h
 * Implementation of Node and related classes' inline routines
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once
#ifndef NODE_INLINE_H
#define NODE_INLINE_H

/** We can't create nodes separately, do it through newNode method of graph */
inline Node::Node( Graph *_graph_p, GraphUid _id):uid(_id), graph_p( _graph_p), element()
{
    first_edge[ GRAPH_DIR_UP] = NULL;
    first_edge[ GRAPH_DIR_DOWN] = NULL;
}

/**
 * Detach myself from graph's node list
 */
inline void Node::detachFromGraph()
{
    detach();
}

/** Return corresponding element */
inline QDomElement Node::elem() const
{
    return element;
}
/**
 * Set element
 */
inline void Node::setElement( QDomElement elem)
{
    element = elem;
}

/**
 * Get node's unique ID
 */
inline GraphUid Node::id() const
{
    return uid;
}

/**
 * Get node's corresponding graph
 */
inline Graph * Node::graph() const
{
    return graph_p;
}
/**
 * Next node in graph's list
 */
inline Node* Node::nextNode()
{
    return next();
}

/**
 * Prev node in graph's list
 */
inline Node* Node::prevNode()
{
    return prev();
}

/**
 * Add predecessor edge
 */
inline void Node::AddPred( Edge *edge)
{
    AddEdgeInDir( edge, GRAPH_DIR_UP);
}

/**
 * Add successor edge
 */
inline void Node::AddSucc( Edge *edge) 
{
    AddEdgeInDir( edge, GRAPH_DIR_DOWN);
}
/**
 * First edge in given direction
 */
inline Edge* Node::firstEdgeInDir( GraphDir dir)
{
    return first_edge[ dir];
}
/** 
 * First successor edge
 */
inline Edge* Node::firstSucc()
{
    return firstEdgeInDir( GRAPH_DIR_DOWN);
}
/** 
 * First predecessor edge
 */
inline Edge* Node::firstPred()
{
    return firstEdgeInDir( GRAPH_DIR_UP);
}

/**
 * delete predecessor edge
 */
inline void Node::deletePred( Edge* edge)
{
    deleteEdgeInDir( GRAPH_DIR_UP, edge);
}

/**
 * delete successor edge
 */
inline void Node::deleteSucc( Edge* edge)
{
    deleteEdgeInDir( GRAPH_DIR_DOWN, edge);
}

/**
 * Create iterator for first succ
 */
inline Node::Succ Node::succsBegin()
{
    return Succ( this);
}

/**
 * Create iterator pointing to succ end
 */
inline Node::Succ Node::succsEnd()
{
    return Succ();
}
/**
 * Create iterator for first succ
 */
inline Node::Pred Node::predsBegin()
{
    return Pred( this);
}
/**
 * Create iterator pointing to succ end
 */
inline Node::Pred Node::predsEnd()
{
    return Pred();
}

/**
 * Create iterator for first succ
 */
inline Node::EdgeIter Node::edgesBegin()
{
    return EdgeIter( this);
}
/**
 * Create iterator pointing to succ end
 */
inline Node::EdgeIter Node::edgesEnd()
{
    return EdgeIter();
}

/**
 * Add an edge to this node in specified direction
 */
inline void
Node::AddEdgeInDir( Edge *edge, GraphDir dir)
{
    assertd( isNotNullP( edge));
    GRAPH_ASSERTD( (int) GRAPH_DIR_DOWN == (int) EDGE_LIST_SUCCS,
                   "Enums of direction and edge lists are not having right values");
    GRAPH_ASSERTD( (int) GRAPH_DIR_UP == (int) EDGE_LIST_PREDS,
                   "Enums of direction and edge lists are not having right values");
    edge->attach( dir, first_edge[ dir]); 
    first_edge[ dir] = edge;
}

/**
 * delete edge pointed by iterator in specidied direction
 */
inline void
Node::deleteEdgeInDir( GraphDir dir, Edge* edge)
{
    assertd( isNotNullP( edge));
    if( first_edge[ dir] == edge)
    {
        first_edge[ dir] = edge->nextEdgeInDir( dir);
    }
}

/**
 * Update DOM tree element
 */
inline void
Node::updateElement()
{
    element.setAttribute( "id", id());
}

/**
 * read properties from DOM tree element
 */
inline void
Node::readFromElement( QDomElement e)
{
    element = e;
}

/** Default Constructor: creates 'end' iterator */
template < class EdgeIterImpl>
inline EdgeIterIface< EdgeIterImpl>::EdgeIterIface()
{

}

/** Constructor from node: iterator points on first edge, i.e. 'begin' iterator */
template < class EdgeIterImpl>
inline EdgeIterIface< EdgeIterImpl>::EdgeIterIface( Node *n):
    impl( n)
{

}

/** Copy constructor */
template < class EdgeIterImpl> 
inline EdgeIterIface< EdgeIterImpl>::EdgeIterIface( const EdgeIterIface& proto)
{
    impl = proto.impl;
}

/** Destructor */
template < class EdgeIterImpl> 
inline EdgeIterIface< EdgeIterImpl>::~EdgeIterIface()
{

}

/** Postincrement operator */
template < class EdgeIterImpl>
inline EdgeIterIface< EdgeIterImpl> 
EdgeIterIface< EdgeIterImpl>::operator++( int)
{
	EdgeIterIface tmp = *this;
    ++*this;
    return tmp;
}

/** Dereferenece operator*/
template < class EdgeIterImpl>
inline Edge * 
EdgeIterIface< EdgeIterImpl>::operator*()
{
    return impl.edge();
}
    
/** Comparison operator */
template < class EdgeIterImpl> 
inline bool
EdgeIterIface< EdgeIterImpl>::operator==(const EdgeIterIface< EdgeIterImpl>& o) const
{
    return impl == o.impl;
} 

/** Not equals operator */
template < class EdgeIterImpl>
inline bool
EdgeIterIface< EdgeIterImpl>::operator!=(const EdgeIterIface< EdgeIterImpl>& o) const
{
    return !(*this == o);
}

/** Get Edge */
template < class EdgeIterImpl>
inline Edge *
EdgeIterIface< EdgeIterImpl>::edge() const
{
    return impl.edge();
}

/** Get Edge */
template < class EdgeIterImpl>
inline Node *
EdgeIterIface< EdgeIterImpl>::node() const
{
    return impl.node();
}

/** Preincrement operator */
template < class EdgeIterImpl>
inline EdgeIterIface< EdgeIterImpl> & 
EdgeIterIface< EdgeIterImpl>::operator++()
{
    GRAPH_ASSERTD( isNotNullP( impl.edge()), "Edge iterator is at end ( NULL in edge pointer)");
    impl.nextEdge();
    return *this;
}

/** Next pred */
inline void PredIterImpl::nextEdge()
{
    GRAPH_ASSERTD( isNotNullP( edge_p), "Edge iterator is at end ( NULL in edge_p pointer)");
    edge_p = edge_p->nextPred();
}


/** Next succ */
inline void SuccIterImpl::nextEdge()
{
    GRAPH_ASSERTD( isNotNullP( edge_p), "Edge iterator is at end ( NULL in edge_p pointer)");
    edge_p = edge_p->nextSucc();
}

/** Preincrement operator */
inline void UnDirIterImpl::nextEdge()
{
    GRAPH_ASSERTD( isNotNullP( edge_p), "Edge iterator is at end ( NULL in edge_p pointer)");
    
    if ( is_pred && isNullP( edge_p->nextPred()))
    {
        is_pred = false;
        edge_p = edge_p->succ()->firstSucc();
    } else
    {
        if ( is_pred)
        {
            edge_p = edge_p->nextPred();
        } else
        {
            edge_p = edge_p->nextSucc();
        }
    }
}

/** Get node on the end of edge */
inline Node * SuccIterImpl::node() const
{
    return edge()->succ();
}

/** Get node on the end of edge */
inline Node * PredIterImpl::node() const
{
    return edge()->pred();
}

/** Get node in UnDir traversal of node's edges */
inline Node * UnDirIterImpl::node() const
{
    if ( is_pred)
    {
        return edge()->pred();
    } else
    {
        return edge()->succ();
    }    
}

/** Constructor gets first succ */
inline SuccIterImpl::SuccIterImpl( Node *n)
{
    edge_p = n->firstSucc();
}

/** Constructor gets first pred */
inline PredIterImpl::PredIterImpl( Node *n)
{
    edge_p = n->firstPred();
}


/** Constructor gets first edge for undirected edges iteration */
inline UnDirIterImpl::UnDirIterImpl( Node *n)
{
    edge_p = n->firstPred();
    is_pred = true;
    if ( isNullP( edge_p)) 
    {
        is_pred = false;
        edge_p = n->firstSucc();
    } 
}

#endif /* NODE_INLINE_H */
