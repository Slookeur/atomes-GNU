/* This file is part of the 'atomes' software

'atomes' is free software: you can redistribute it and/or modify it under the terms
of the GNU Affero General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

'atomes' is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with 'atomes'.
If not, see <https://www.gnu.org/licenses/>

Copyright (C) 2022-2026 by CNRS and University of Strasbourg */

/*!
* @file optim_edit.c
* @short Functions to create the geometry optimization window
* @author Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr>
*/

/*
* This file: 'optim_edit.c'
*
* Contains:
*

 - The functions to create the geometry optimization window

*
* List of functions:

  gboolean is_atom_win_active (glwin * view);

  G_MODULE_EXPORT gboolean delete_optimize (GtkWindow * widg, gpointer data);
  G_MODULE_EXPORT gboolean delete_optimize (GtkWidget * widg, GdkEvent * event, gpointer data);

  void clean_optim_window (project * this_proj);
  void clean_optim_win (project * this_proj);
  void prepare_geometry_optimization (gpointer data, gboolean visible);

  G_MODULE_EXPORT void close_optimize (GtkButton * but, gpointer data);
  G_MODULE_EXPORT void set_reset_optimization (GtkCheckButton * but, gpointer data);
  G_MODULE_EXPORT void set_reset_optimization (GtkToggleButton * but, gpointer data);
  G_MODULE_EXPORT void optim_edit (GtkButton * but, gpointer data);
  G_MODULE_EXPORT void optimize_window (GSimpleAction * action, GVariant * parameter, gpointer data);
  G_MODULE_EXPORT void optimize_window (GtkWidget * widg, gpointer data);

  GtkWidget * create_optimization_window (project * this_proj);

*/

#include "global.h"
#include "atom_edit.h"

extern G_MODULE_EXPORT void optimize_geometry (GtkButton * but, gpointer data);

/* 
 * 
 * Si j'ai bien compris ton code Q, E, S sont temporaire (et identique pour tous les atomes)
 * Dans un premier temps j'ai adapté l'Interface Utilisateur d'atomes pour te permettre 
 * d'ajuster ces valeurs, attention de spécifier des valeurs limites pour ces paramètres MIN_ et MAX_
 * pour éviter les calculs impossibles, à toi de voir quoi faire
 * Idem pour les paramètres généraux concernant la minimisation, il est nécessaire de prévoir
 * quelles gammes de valeurs sont physiquement réalistes, et de ne pas laisser l'utilisateur 
 * rentrer quelque chose d'abhérent, adapter les lignes suivantes pour faire cela:
 *
 * */
// min, max for default atomic parameters
#define MIN_Q -5.0
#define MAX_Q  5.0
#define MIN_E -100000
#define MAX_E  100000
#define MIN_S -100000
#define MAX_S  100000
// min, max for minimisation parameters
#define MIN_Ft -100000
#define MAX_Ft  100000
#define MIN_Et -100000
#define MAX_Et  100000
#define MAX_Sl  100000

/*
 * J'ai fait en sorte de laisser la liberté de modifier les valeurs par défaut, 
 * qu'ils faut prévoir de toute façon (est-ce fonction du système ?, d'autre chose ?
 * faut il faire quelque chose pour les déterminer ?
 * En attendant tu peut ajuser les champs suivants pour définir ces valeurs par défaut:
 */
// Default values
#define DEFAULT_Q 1.0
#define DEFAULT_E 1.0
#define DEFAULT_S 1.0
#define DEFAULT_DT 1.5
#define DEFAULT_FT 5.0
#define DEFAULT_ET 5.0
#define DEFAULT_SL 10.0
#define DEFAULT_NI 1000

/*!
  \fn gboolean is_optim_win_active (glwin * view)

  \brief is the geometry optimization window visible ?

  \param view the target glwin
*/
gboolean is_optim_win_active (glwin * view)
{
  if (view -> optim_win)
  {
    return view -> optim_win -> visible;
  }
  return FALSE;
}

/*!
  \fn void clean_optim_win (project * this_proj)

  \brief clean geometry optimization data

  \param this_proj the target project
*/
void clean_optim_win (project * this_proj)
{
  this_proj -> modelgl -> optim_win -> win = destroy_this_widget (this_proj -> modelgl -> optim_win -> win);
  g_free (this_proj -> modelgl -> optim_win);
  this_proj -> modelgl -> optim_win = NULL;
  int shaders[6] = {ATOMS, BONDS, POLYS, RINGS, SELEC, VOLMS};
  re_create_md_shaders (6, shaders, this_proj);
  this_proj -> modelgl -> create_shaders[LABEL] = TRUE;
  this_proj -> modelgl -> create_shaders[MEASU] = TRUE;
  this_proj -> modelgl -> create_shaders[MAXIS] = TRUE;
  this_proj -> modelgl -> create_shaders[PICKS] = TRUE;
  int i;
  for (i=0; i<2; i++)
  {
    if (this_proj -> modelgl -> saved_coord[i] != NULL)
    {
      g_free (this_proj -> modelgl -> saved_coord[i]);
      this_proj -> modelgl -> saved_coord[i] = NULL;
    }
  }
  clean_other_window_after_edit (this_proj);
  update (this_proj -> modelgl);
}

/*!
  \fn G_MODULE_EXPORT void close_optimize (GtkButton * but, gpointer data)

  \brief close optimization window

  \param but the GtkButton sending the signal
  \param data the associated data pointer
*/
G_MODULE_EXPORT void close_optimize (GtkButton * but, gpointer data)
{
  int id = GPOINTER_TO_INT(data);
  project * this_proj = get_project_by_id(id);
  gboolean leave = (this_proj -> modelgl -> was_optimized) ? ask_yes_no(_("Leaving without saving ?"),
                                                                        _("To preserve atom(s) displacement(s) press the 'Apply' button\n"
                                                                          "Otherwise initial atom positions will be restored ...\n"
                                                                          "\t\t\t Are you sure to leave ?") ,
                                                                        GTK_MESSAGE_QUESTION, this_proj -> modelgl -> optim_win -> win) : TRUE;
  if (leave && this_proj -> modelgl -> optim_win)
  {
    int h;
    for (h=0; h<2; h++)
    {
      reset_coordinates (this_proj, h);
      init_coordinates (this_proj, h, FALSE, FALSE);
    }
    this_proj -> modelgl -> was_optimized = FALSE;
    clean_optim_win (this_proj);
  }
}

#ifdef GTK4
/*!
  \fn G_MODULE_EXPORT gboolean delete_optimize (GtkWindow * widg, gpointer data)

  \brief geometry optimization window close event callback GTK4

  \param widg
  \param data the associated data pointer
*/
G_MODULE_EXPORT gboolean delete_optimize (GtkWindow * widg, gpointer data)
#else
/*!
  \fn G_MODULE_EXPORT gboolean delete_optimize (GtkWidget * widg, GdkEvent * event, gpointer data)

  \brief geometry optimization window close event callback GTK3

  \param widg the GtkWidget sending the signal
  \param event the GdkEvent triggering the signal
  \param data the associated data pointer
*/
G_MODULE_EXPORT gboolean delete_optimize (GtkWidget * widg, GdkEvent * event, gpointer data)
#endif
{
  close_optimize (NULL, data);
  return TRUE;
}

/*!
  \fn G_MODULE_EXPORT void reset_optimization (GtkToggleButton * but, gpointer data)

  \brief reset model optimization callback

  \param but the GtkButton sending the signal
  \param data the associated data pointer
*/
G_MODULE_EXPORT void reset_optimization (GtkToggleButton * but, gpointer data)
{
  int pid = GPOINTER_TO_INT(data);
  project * this_proj = get_project_by_id (pid);
  if (ask_yes_no(_("Reset"), _("Reset and get back to initial coordinates ?\n This will affect all atoms !"),
                  GTK_MESSAGE_WARNING, this_proj -> modelgl -> optim_win -> win))
  {
    int h;
    for (h=0; h<2; h++)
    {
      reset_coordinates (this_proj, h);
      init_coordinates (this_proj, h, FALSE, FALSE);
    }
    init_default_shaders (this_proj -> modelgl);
    update (this_proj -> modelgl);
    widget_set_sensitive (this_proj -> modelgl -> optim_win -> reset_but, FALSE);
  }
}

/*!
  \fn G_MODULE_EXPORT void apply_optimize (GtkButton * but, gpointer data)

  \brief apply geometry optimization callback

  \param but the GtkButton sending the signal
  \param data the associated data pointer
*/
G_MODULE_EXPORT void apply_optimize (GtkButton * but, gpointer data)
{
  int id = GPOINTER_TO_INT(data);
  project * this_proj = get_project_by_id(id);
  int h, i, j;
  i = this_proj -> modelgl -> mode;
  this_proj -> modelgl -> mode = ANALYZE;
  image * last = this_proj -> modelgl -> anim -> last -> img;
  vec4_t q = last -> rotation_quaternion;
  j = -1;
  if (id != activep)
  {
    i = activep;
    active_project_changed (id);
  }
  init_camera (this_proj);
  if (j != -1) active_project_changed (j);
  last -> rotation_quaternion = q;
  this_proj -> modelgl -> mode = i;
  for (h=0; h<2; h++)
  {
    g_free (this_proj -> modelgl -> saved_coord[h]);
    this_proj -> modelgl -> saved_coord[h] = save_coordinates (this_proj, h);
  }
  this_proj -> modelgl -> was_optimized = FALSE;
}

/*!
  \fn G_MODULE_EXPORT void update_optim_param (GtkEntry * entry, gpointer data)

  \brief update geometry optimization parameters

  \param entry the GtkEntry sending the signal
  \param data the associated data pointer
*/
G_MODULE_EXPORT void update_optim_param (GtkEntry * entry, gpointer data)
{
  const gchar * m = entry_get_text (entry);
  double v = string_to_double ((gpointer)m);
  tint * id = (tint *)data;
  project * this_proj = get_project_by_id (id -> a);
  // Besoin de valeurs limites pour ces paramètres et valider l'entrée
  // Il faut éviter de laisser l'utilisateur faire n'importe quoi avant de lancer l'optimisation:
  // -     Q: gamme de valeur [MIN_Q, MAX_Q]
  // -    E0: gamme de valeur [MIN_E, MAX_E]
  // - sigma: gamme de valeur [MIN_S, MAX_S]
  switch (id -> b)
  {
    case 0:
      if (v >= MIN_Q && v <= MAX_Q)
      {
        this_proj -> modelgl -> optim_win -> Q = v;
      }
      update_entry_double (entry, this_proj -> modelgl -> optim_win -> Q);
      break;
    case 1:
      if (v >= MIN_E && v <= MAX_E)
      {
        this_proj -> modelgl -> optim_win -> E0 = v;
      }
      update_entry_double (entry, this_proj -> modelgl -> optim_win -> E0);
      break;
    case 2:
      if (v >= MIN_S && v <= MAX_S)
      {
        this_proj -> modelgl -> optim_win -> sigma = v;
      }
      update_entry_double (entry, this_proj -> modelgl -> optim_win -> sigma);
      break;
  }
}

/*!
  \fn G_MODULE_EXPORT void update_minim_param (GtkEntry * entry, gpointer data)

  \brief update minimisation parameters

  \param entry the GtkEntry sending the signal
  \param data the associated data pointer
*/
G_MODULE_EXPORT void update_minim_param (GtkEntry * entry, gpointer data)
{
  const gchar * m = entry_get_text (entry);
  double v = string_to_double ((gpointer)m);
  tint * id = (tint *)data;
  project * this_proj = get_project_by_id (id -> a);
  // Besoin de valeurs limites pour ces paramètres et valider l'entrée
  // Il faut éviter de laisser l'utilisateur faire n'importe quoi avant de lancer l'optimisation
  // - Ftol: gamme de valeur [MIN_Ft, MAX_Ft]
  // - Etol: gamme de valeur [MIN_Et, MAX_Et]
  // - Idem pour les autres valeurs définies
  switch (id -> b)
  {
    case 0:
      if (v >= 0)
      {
        this_proj -> modelgl -> optim_win -> Dt = v;
      }
      update_entry_double (entry, this_proj -> modelgl -> optim_win -> Dt);
      break;
    case 1:
      if (v >= 0)
      {
        this_proj -> modelgl -> optim_win -> Nitermax = v;
      }
      update_entry_int (entry, this_proj -> modelgl -> optim_win -> Nitermax);
      break;
    case 2:
      if (v >= MIN_Ft && v <= MAX_Ft)
      {
        this_proj -> modelgl -> optim_win -> Ftol = v;
      }
      update_entry_double (entry, this_proj -> modelgl -> optim_win -> Ftol);
      break;
    case 3:
      if (v >= MIN_Et && v <= MAX_Et)
      {
        this_proj -> modelgl -> optim_win -> Etol = v;
      }
      update_entry_double (entry, this_proj -> modelgl -> optim_win -> Etol);
      break;
    case 4:
      if (v >= 0.0 && v <= MAX_Sl)
      {
        this_proj -> modelgl -> optim_win -> startlength = v;
      }
      update_entry_double (entry, this_proj -> modelgl -> optim_win -> startlength);
      break;

  }
}

/*!
  \fn G_MODULE_EXPORT void optim_tunit_changed (GtkComboBox * box, gpointer data)

  \brief change time step units

  \param box the GtkComboBox sending the signal
  \param data the associated data pointer
*/
G_MODULE_EXPORT void optim_tunit_changed (GtkComboBox * box, gpointer data)
{
  geom_opti * optim_win = (geom_opti *)data;
  optim_win -> Dt_unit = combo_get_active ((GtkWidget *)box);
}

/*!
  \fn GtkWidget * create_optimize_window (project * this_proj)

  \brief create the geometry optimization window

  \param this_proj the target project
*/
GtkWidget * create_optimize_window (project * this_proj)
{
  gchar * str = g_strdup_printf (_("Geometry optimization - %s"), this_proj -> name);
  this_proj -> modelgl -> was_optimized = FALSE;
  geom_opti * optim_win = this_proj -> modelgl -> optim_win;
  GtkWidget * win = create_win (str, this_proj -> modelgl -> win, FALSE, FALSE);
  g_free (str);
  optim_win -> vbox = create_vbox (5);
  add_container_child (CONTAINER_WIN, win, optim_win -> vbox);
  gtk_widget_set_size_request (optim_win -> vbox, 450, -1);
  int i, j;
  GtkWidget * hbox;
  GtkWidget * p_entry;
  GtkWidget * tcombo;
  
  abox (optim_win -> vbox, _("<b>Minimisation parameters</b>"), 0);
  gchar * m_name_lab[5]={i18n("Verlet integration time step &#x3b4;t"), 
                         i18n("Maximum number of iterations"),
                         i18n("Target force tolerance"),
                         i18n("Target energy tolerance"),
                         i18n("Max Length ?")};
  gchar * m_unit_lab[4]={" ", 
                         " N (Kg.m.s<sup>-2</sup>)", 
                         i18n(" Unit ?"),
                         " &#xC5;"};
  
  /*
   * Paramètres temporaires, pour le moment ces valeurs seront réinitialiser chaque fois
   * que ouvrira/fermera la fenêtre 'Optimisation de Géométrie', dans le futur je ferais 
   * en sorte que les informations soient prévervées, mais pour me lancer là dedans 
   * il faut que je sois certain de quoi garder ou pas 
  */
  optim_win -> Dt = DEFAULT_DT;
  optim_win -> Dt_unit = 0;
  optim_win -> Nitermax = DEFAULT_NI;
  optim_win -> Ftol = DEFAULT_FT;
  optim_win -> Etol = DEFAULT_ET;
  optim_win -> startlength = DEFAULT_SL;
  for (i=0; i<5; i++)
  {
    hbox = create_hbox (BSEP);
    add_box_child_start (GTK_ORIENTATION_VERTICAL, optim_win -> vbox, hbox, FALSE, FALSE, 0);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, markup_label (_(m_name_lab[i]), 200, -1, 0.0, 0.5), FALSE, FALSE, 20);
    p_entry = create_entry (G_CALLBACK(update_minim_param), 100, 15, FALSE, & this_proj -> modelgl -> colorp[i][0]);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, p_entry, FALSE, FALSE, 5);
    switch (i)
    {
      case 0: 
        update_entry_double (GTK_ENTRY(p_entry), optim_win -> Dt);
        tcombo = create_combo ();
        for (j=0; j<5; j++) combo_text_append (tcombo, untime[j]);
        combo_set_active (tcombo,  optim_win -> Dt_unit);
        g_signal_connect(G_OBJECT(tcombo), "changed", G_CALLBACK(optim_tunit_changed), optim_win);
        add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, tcombo, FALSE, FALSE, 0);
        break;
      case 1:
        update_entry_int (GTK_ENTRY(p_entry), optim_win -> Nitermax);
        break;
      case 2:
        update_entry_double (GTK_ENTRY(p_entry), optim_win -> Ftol);
        break;
      case 3:
        update_entry_double (GTK_ENTRY(p_entry), optim_win -> Etol);
        break;
      case 4:
        update_entry_double (GTK_ENTRY(p_entry), optim_win -> startlength);
        break;
    }
    if (i) add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, markup_label(_(m_unit_lab[i-1]), 15, -1, 0.0, 0.5), FALSE, FALSE, 0);
  }
  
  abox (optim_win -> vbox, _("<b>Temporary atomic default parameters</b>"), 0);
  // Récupération des paramètres Q, E0 et sigma par défaut:
  // Faut il préserver ces champs sur le long terme, ou pas ?
  gchar * p_name_lab[3]={i18n("Charge"), i18n("Minimum well depth potential"), i18n("Distance for null potential")};
  gchar * p_unit_lab[3]={i18n(" Unit ?"), i18n(" Unit ?"), i18n(" &#xC5;")};
  for (i=0; i<3; i++)
  {
    hbox = create_hbox(0);
    add_box_child_start (GTK_ORIENTATION_VERTICAL, optim_win -> vbox, hbox, FALSE, FALSE, 0);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, markup_label (_(p_name_lab[i]), 200, -1, 0.0, 0.5), FALSE, FALSE, 20);
    p_entry = create_entry (G_CALLBACK(update_optim_param), 100, 15, FALSE, & this_proj -> modelgl -> colorp[i][0]);
    switch (i)
    {
      case 0: 
        update_entry_double (GTK_ENTRY(p_entry), optim_win -> Q);
        break;
      case 1:
        update_entry_double (GTK_ENTRY(p_entry), optim_win -> E0);
        break;
      case 2:
        update_entry_double (GTK_ENTRY(p_entry), optim_win -> sigma);
        break;
    }
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, p_entry, FALSE, FALSE, 5);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, markup_label(_(p_unit_lab[i]), 15, -1, 0.0, 0.5), FALSE, FALSE, 0);
  }

  // Button pour lancer l'optimisation
  GtkWidget * but = create_button (_("Optimize"), IMG_STOCK, APPLY, 150, -1, GTK_RELIEF_NORMAL, G_CALLBACK(optimize_geometry), GINT_TO_POINTER(this_proj -> id));
  hbox = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, optim_win -> vbox, hbox, FALSE, FALSE, 10);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, hbox, but, TRUE, FALSE, 0);

  add_gtk_close_event (win, G_CALLBACK(delete_optimize), GINT_TO_POINTER(this_proj -> id));
  hbox = create_hbox (5);
  add_box_child_end (optim_win -> vbox, hbox, TRUE, FALSE, 0);
  but = create_button (_("Apply"), IMG_STOCK, APPLY, -1, -1, GTK_RELIEF_NORMAL, G_CALLBACK(apply_optimize), GINT_TO_POINTER(this_proj -> id));
  add_box_child_end (hbox, but, FALSE, FALSE, 5);
  optim_win -> reset_but = create_button (_("Reset"), IMG_STOCK, (gpointer)MEDIA_LOOP, -1, -1, GTK_RELIEF_NORMAL, 
                                          G_CALLBACK(reset_optimization), GINT_TO_POINTER(this_proj -> id));
  widget_set_sensitive (optim_win -> reset_but, FALSE);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, optim_win -> reset_but, FALSE, FALSE, 5);
  but = create_button (_("Close"), IMG_STOCK, FCLOSE, -1, -1, GTK_RELIEF_NORMAL, G_CALLBACK(close_optimize), GINT_TO_POINTER(this_proj -> id));
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, but, FALSE, FALSE, 5);
  return win;
}

/*!
  \fn void prepare_geometry_optimization (gpointer data, gboolean visible)

  \brief prepare geometry optimization

  \param data the associated data pointer
  \param visible is the window visible (1/0)
*/
void prepare_geometry_optimization (gpointer data, gboolean visible)
{
  tint * id = (tint *) data;
  int i;
  project * this_proj = get_project_by_id(id -> a);
  if (this_proj -> modelgl -> atom_win == NULL)
  {
    if (this_proj -> modelgl -> optim_win == NULL)
    {
      this_proj -> modelgl -> optim_win = g_malloc0(sizeof*this_proj -> modelgl -> optim_win);
      for (i=0; i<2; i++)
      {
        init_coordinates (this_proj, i, FALSE, FALSE);
      }
    }
    this_proj -> modelgl -> optim_win -> visible = visible;
    if (visible)
    {
      this_proj -> modelgl -> optim_win -> win = create_optimize_window (this_proj);
      show_the_widgets (this_proj -> modelgl -> optim_win -> win);
      if (this_proj -> modelgl -> mode == EDITION)
      {
#ifdef GTK4
        set_mode (NULL, & this_proj -> modelgl -> colorp[0][0]);
#else
        // GTK3 Menu Action To Check
        gtk_check_menu_item_set_active ((GtkCheckMenuItem *)this_proj -> modelgl -> ogl_mode[0], TRUE);
        set_mode (this_proj -> modelgl -> ogl_mode[0], & this_proj -> modelgl -> colorp[0][0]);
#endif
      }
    }
  }
  else
  {
    show_warning ("You need to close the \"Model Edition\" window first !", this_proj -> modelgl -> win);
  }
}

#ifdef GTK4
/*!
  \fn G_MODULE_EXPORT void optimize_window (GSimpleAction * action, GVariant * parameter, gpointer data)

  \brief geometry optimisation window window callback GTK4

  \param action the GAction sending the signal
  \param parameter GVariant parameter of the GAction, if any
  \param data the associated data pointer
*/
G_MODULE_EXPORT void optimize_window (GSimpleAction * action, GVariant * parameter, gpointer data)
#else
/*!
  \fn G_MODULE_EXPORT void optimize_window (GtkWidget * widg, gpointer data)

  \brief geometry optimisation window callback GTK3

  \param widg the GtkWidget sending the signal
  \param data the associated data pointer
*/
G_MODULE_EXPORT void optimize_window (GtkWidget * widg, gpointer data)
#endif
{
  prepare_geometry_optimization (data, TRUE);
}
