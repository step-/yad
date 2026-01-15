/*
 * This file is part of YAD.
 *
 * YAD is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * YAD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with YAD. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2008-2019, Victor Ananjevsky <ananasik@gmail.com>
 * Copyright (C) 2019-2026, step https://github.com/step-
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <gdk/gdk.h>

#include "yad.h"

static GtkWidget *paned;

GtkWidget *
paned_create_widget (GtkWidget * dlg)
{
  GtkWidget *w, *s;

  /* get shared memory */
  tabs = get_tabs (options.common_data.key, TRUE);
  if (!tabs)
    exit (-1);

  /* create widget */
#if !GTK_CHECK_VERSION(3,0,0)
  if (options.paned_data.orient == GTK_ORIENTATION_HORIZONTAL)
    paned = w = gtk_hpaned_new ();
  else
    paned = w = gtk_vpaned_new ();
#else
  paned = w = gtk_paned_new (options.paned_data.orient);
#endif
  gtk_widget_set_name (w, "yad-paned-widget");

  s = gtk_socket_new ();
  gtk_widget_set_can_focus (s, TRUE);
  gtk_paned_add1 (GTK_PANED (w), s);
  g_object_set_data (G_OBJECT (w), "s1", s);

  s = gtk_socket_new ();
  gtk_widget_set_can_focus (s, TRUE);
  gtk_paned_add2 (GTK_PANED (w), s);
  g_object_set_data (G_OBJECT (w), "s2", s);

  return w;
}

static void
paned_focus_pane (gint n)
{
  GtkWidget *s1, *s2, *target = NULL;

  if (n < 1 || n > 2)
    {
      if (options.debug)
        g_printerr (_("WARNING: wrong focused pane number %d. Must be 1 or 2\n"), n);
      return;
    }

  s1 = GTK_WIDGET (g_object_get_data (G_OBJECT (paned), "s1"));
  s2 = GTK_WIDGET (g_object_get_data (G_OBJECT (paned), "s2"));

  if (n == 1 && s1)
    {
      if (GTK_IS_CONTAINER (s1) && !(target = gtk_container_get_focus_child (GTK_CONTAINER (s1))))
        target = gtk_widget_get_can_focus (s1) ? s1 : NULL;
    }
  else if (n == 2 && s2)
    {
      if (GTK_IS_CONTAINER (s2) && !(target = gtk_container_get_focus_child (GTK_CONTAINER (s2))))
        target = gtk_widget_get_can_focus (s2) ? s2 : NULL;
    }

  if (target && gtk_widget_get_visible (target))
    gtk_widget_grab_focus (target);
  else if (options.debug)
    g_printerr (_("WARNING: No focusable widget in pane %d\n"), n);
}

void
paned_swallow_childs (void)
{
  GtkWidget *s1, *s2;

  s1 = GTK_WIDGET (g_object_get_data (G_OBJECT (paned), "s1"));
  s2 = GTK_WIDGET (g_object_get_data (G_OBJECT (paned), "s2"));

  /* wait until all children are registered */
  while (!tabs[1].xid || !tabs[2].xid)
    usleep (1000);

  if (tabs[1].pid != -1)
    gtk_socket_add_id (GTK_SOCKET (s1), tabs[1].xid);
  if (tabs[2].pid != -1)
    gtk_socket_add_id (GTK_SOCKET (s2), tabs[2].xid);

  /* must be after embedding children */
  if (options.paned_data.splitter > 0)
    gtk_paned_set_position (GTK_PANED (paned), options.paned_data.splitter);

  paned_focus_pane (options.paned_data.focused);
}

void
paned_print_result (void)
{
  if (tabs[1].pid != -1)
    kill (tabs[1].pid, SIGUSR1);
  if (tabs[2].pid != -1)
    kill (tabs[2].pid, SIGUSR1);
}

void
paned_close_childs (void)
{
  guint i;
  struct shmid_ds buf;
  gboolean is_running = TRUE;

  if (tabs[1].pid != -1)
    kill (tabs[1].pid, SIGUSR2);
  if (tabs[2].pid != -1)
    kill (tabs[2].pid, SIGUSR2);

  /* wait for stop subprocesses */
  while (is_running)
    {
      is_running = FALSE;
      for (i = 1; i < 3; i++)
        {
          if (tabs[i].pid != -1 && kill (tabs[i].pid, 0) == 0)
            {
              is_running = TRUE;
              break;
            }
        }
      if (is_running)
        usleep (1000);
    }

  /* cleanup shared memory */
  shmctl (tabs[0].pid, IPC_RMID, &buf);
  shmdt (tabs);
}
