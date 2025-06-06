/* This file is part of the 'atomes' software

'atomes' is free software: you can redistribute it and/or modify it under the terms
of the GNU Affero General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

'atomes' is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with 'atomes'.
If not, see <https://www.gnu.org/licenses/>

Copyright (C) 2022-2025 by CNRS and University of Strasbourg */

/*!
* @file read_vas.c
* @short Functions to read VASP atomic coordinates
* @author Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr>
*/

/*
* This file: 'read_vas.c'
*
* Contains:
*

 - The functions to read VASP atomic coordinates

*
* List of functions:

  int vas_get_atom_coordinates (int sli);
  int open_vas_file (int linec);

*/

#include "global.h"
#include "glview.h"
#include "callbacks.h"
#include "interface.h"
#include "project.h"
#include "bind.h"
#include "readers.h"
#ifdef OPENMP
#  include <omp.h>
#endif

/*!
  \fn int vas_get_atom_coordinates (int sli)

  \brief get the atomic coordinates from the VASP file

  \param sli Correction parameters depending on the VASP file version
*/
int vas_get_atom_coordinates (int sli)
{
  int i, j, k, l;
  gchar * lia[4] = {"a", "b", "c", "d"};
  allocatoms (active_project);
#ifdef OPENMP
  int res;
  int numth = omp_get_max_threads ();
  gboolean doatoms =  FALSE;
  gchar * saved_line;
  if (active_project -> steps < numth)
  {
    if (numth >= 2*(active_project -> steps-1))
    {
      doatoms = TRUE;
    }
    else
    {
      numth = active_project -> steps;
    }
  }
  if (doatoms)
  {
    // OpenMP on atoms
    res = 0;
    for (i=0; i<active_project -> steps; i++)
    {
      k = 1 + i*(active_project -> natomes + 1) + sli;
      #pragma omp parallel for num_threads(numth) private(j,this_line,saved_line,this_word) shared(i,k,lia,coord_line,active_project,res)
      for (j=0; j<active_project -> natomes; j++)
      {
        if (res == 2) goto enda;
        this_line = g_strdup_printf ("%s", coord_line[k+j]);
        saved_line = g_strdup_printf ("%s", this_line);
        this_word = strtok_r (this_line, " ", & saved_line);
        if (! this_word)
        {
          format_error (i+1, j+1, lia[0], k+j);
          res = 2;
          goto enda;
        }
        active_project -> atoms[i][j].x = string_to_double ((gpointer)this_word);
        this_word = strtok_r (NULL, " ", & saved_line);
        if (! this_word)
        {
          format_error (i+1, j+1, lia[1], k+j);
          res = 2;
          goto enda;
        }
        active_project -> atoms[i][j].y = string_to_double ((gpointer)this_word);
        this_word = strtok_r (NULL, " ", & saved_line);
        if (! this_word)
        {
          format_error (i+1, j+1, lia[2], k+j);
          res = 2;
          goto enda;
        }
        active_project -> atoms[i][j].z = string_to_double ((gpointer)this_word);
        g_free (this_line);
        enda:;
      }
      if (res == 2) break;
    }
  }
  else
  {
    res = 0;
    #pragma omp parallel for num_threads(numth) private(i,j,k,this_line,saved_line,this_word) shared(lia,coord_line,active_project,res)
    for (i=0; i<active_project -> steps; i++)
    {
      if (res == 2) goto ends;
      k = 1 + i*(active_project -> natomes + 1) + sli;
      for (j=0; j<active_project -> natomes; j++)
      {
        this_line = g_strdup_printf ("%s", coord_line[k+j]);
        saved_line = g_strdup_printf ("%s", this_line);
        this_word = strtok_r (this_line, " ", & saved_line);
        if (! this_word)
        {
          format_error (i+1, j+1, lia[0], k+j);
          res = 2;
          goto ends;
        }
        active_project -> atoms[i][j].x = string_to_double ((gpointer)this_word);
        this_word = strtok_r (NULL, " ", & saved_line);
        if (! this_word)
        {
          format_error (i+1, j+1, lia[1], k+j);
          res = 2;
          goto ends;
        }
        active_project -> atoms[i][j].y = string_to_double ((gpointer)this_word);
        this_word = strtok_r (NULL, " ", & saved_line);
        if (! this_word)
        {
          format_error (i+1, j+1, lia[2], k+j);
          res = 2;
          goto ends;
        }
        active_project -> atoms[i][j].z = string_to_double ((gpointer)this_word);
        g_free (this_line);
      }
      ends:;
    }
  }
  g_free (coord_line);
  if (res == 2) return 2;
  for (i=1; i<active_project -> steps; i++)
  {
    for (j=0; j<active_project -> natomes; j++)
    {
      if (active_project -> atoms[i-1][j].sp != active_project -> atoms[i][j].sp)
      {
        add_reader_info (g_strdup_printf ("Error - chemical species changes between steps %d and %d, for atom %d !", i, i+1, j+1), 0);
        return 2;
      }
    }
  }
#else
  line_node * tmp_line;
  tail = head;
  k = 0;
  for (i=0; i<sli; i++)
  {
    tmp_line = tail;
    tail = tail -> next;
    g_free (tmp_line);
    k ++;
  }
  for (i=0; i<active_project -> steps; i++)
  {
    tmp_line = tail;
    tail = tail -> next;
    g_free (tmp_line);
    k ++;
    for (j=0; j<active_project -> natomes; j++)
    {
      this_line = g_strdup_printf ("%s", tail -> line);
      this_word = strtok (this_line, " ");
      if (! this_word)
      {
        format_error (i+1, j+1, lia[0], k);
        return 2;
      }
      active_project -> atoms[i][j].x = string_to_double ((gpointer)this_word);
      this_word = strtok (NULL, " ");
      if (! this_word)
      {
        format_error (i+1, j+1, lia[1], k);
        return 2;
      }
      active_project -> atoms[i][j].y = string_to_double ((gpointer)this_word);
      this_word = strtok (NULL, " ");
      if (! this_word)
      {
        format_error (i+1, j+1, lia[2], k);
        return 2;
      }
      active_project -> atoms[i][j].z = string_to_double ((gpointer)this_word);
      tmp_line = tail;
      tail = tail -> next;
      g_free (tmp_line);
      k ++;
    }
  }
#endif
  i = 0;
  for (j=0; j<this_reader -> nspec; j++)
  {
    for (k=0; k<this_reader -> nsps[j]; k++)
    {
      for (l=0; l<active_project -> steps; l++)
      {
        active_project -> atoms[l][i].sp = j;
      }
      i ++;
    }
  }
  return 0;
}

/*!
  \fn int open_vas_file (int linec)

  \brief open VASP file

  \param linec Number of lines in the file
*/
int open_vas_file (int linec)
{
  int i, j;
  j = 0;
  for (i=5; i<8; i++)
  {
    if ((linec - i)%(this_reader -> natomes + 1) == 0) j = i;
  }
  if (! j) return 2;
  reader_info ("xdatcar", "Number of atoms", this_reader -> natomes);
  active_project -> steps = (linec-j) / (this_reader -> natomes + 1);
  reader_info ("xdatcar", "Number of steps", active_project -> steps);
  active_project -> natomes = this_reader -> natomes;
  return vas_get_atom_coordinates (j);
}

