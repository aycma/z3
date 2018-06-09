/**++
Copyright (c) Arie Gurfinkel

Module Name:

    qe_term_graph.h

Abstract:

    Equivalence graph of terms

Author:

    Arie Gurfinkel

Notes:

--*/
#ifndef QE_TERM_GRAPH_H__
#define QE_TERM_GRAPH_H__

#include "ast/ast.h"
#include "util/plugin_manager.h"

namespace qe {

    class term;

    class term_graph_plugin {
        family_id m_id;
    public:
        term_graph_plugin(family_id fid) : m_id(fid) {}
        virtual ~term_graph_plugin() {}

        family_id get_family_id() const {return m_id;}
        
        /// Process (and potentially augment) a literal
        virtual app_ref process_lit (app *lit) = 0;
    };
    
    class term_graph {
        ast_manager &    m;
        ptr_vector<term> m_terms;
        app_ref_vector   m_lits;
        u_map<term* >    m_app2term;        
        app_ref_vector   m_pinned;
        u_map<expr*>      m_term2app;
        
        plugin_manager<term_graph_plugin> m_plugins;
        
        void merge(term &t1, term &t2);
        
        term *mk_term(expr *t);
        term *get_term(expr *t);
        
        term *internalize_term(expr *t);
        void internalize_eq(expr *a1, expr *a2);
        void internalize_lit(app *lit);
        
        bool is_internalized(app *a);
        
        bool term_le(term const &t1, term const &t2);
        void pick_root (term &t);
        void pick_roots();
        
        void reset_marks();
        
        expr* mk_app_core(expr* a);
        expr_ref mk_app(term const &t);
        expr_ref mk_app(expr *a);
        void mk_equalities(term const &t, app_ref_vector &out);
        void mk_all_equalities(term const &t, app_ref_vector &out);
        void display(std::ostream &out);        
    public:
        term_graph(ast_manager &man);
        ~term_graph();
        
        ast_manager& get_ast_manager() const { return m;}
        
        void add_lit(app *lit);
        void add_lits(expr_ref_vector const &lits) {
            for (expr* e : lits) add_lit(::to_app(e));
        }
        void add_eq(expr* a, expr* b);
        
        void reset();
        void to_lits(app_ref_vector &lits, bool all_equalities = false);
        void to_lits(expr_ref_vector &lits, bool all_equalities = false);
        app_ref to_app();

        /**
         * Return literals obtained by projecting added literals 
         * onto the vocabulary of decls (if exclude is false) or outside the 
         * vocabulary of decls (if exclude is true).
         */
         expr_ref_vector project(func_decl_ref_vector const& decls, bool exclude);
        
    };

}
#endif
