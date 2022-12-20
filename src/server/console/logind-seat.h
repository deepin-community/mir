/*
 * Generated by gdbus-codegen 2.56.0. DO NOT EDIT.
 *
 * The license of this code is the same as for the D-Bus interface description
 * it was derived from.
 */

#ifndef __LOGIND_SEAT_H__
#define __LOGIND_SEAT_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for org.freedesktop.login1.Seat */

#define LOGIND_TYPE_SEAT (logind_seat_get_type ())
#define LOGIND_SEAT(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), LOGIND_TYPE_SEAT, LogindSeat))
#define LOGIND_IS_SEAT(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), LOGIND_TYPE_SEAT))
#define LOGIND_SEAT_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), LOGIND_TYPE_SEAT, LogindSeatIface))

struct _LogindSeat;
typedef struct _LogindSeat LogindSeat;
typedef struct _LogindSeatIface LogindSeatIface;

struct _LogindSeatIface
{
  GTypeInterface parent_iface;


  gboolean (*handle_activate_session) (
    LogindSeat *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_unnamed_arg0);

  gboolean (*handle_switch_to) (
    LogindSeat *object,
    GDBusMethodInvocation *invocation,
    guint arg_unnamed_arg0);

  gboolean (*handle_switch_to_next) (
    LogindSeat *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_switch_to_previous) (
    LogindSeat *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_terminate) (
    LogindSeat *object,
    GDBusMethodInvocation *invocation);

  GVariant * (*get_active_session) (LogindSeat *object);

  gboolean  (*get_can_graphical) (LogindSeat *object);

  gboolean  (*get_can_multi_session) (LogindSeat *object);

  gboolean  (*get_can_tty) (LogindSeat *object);

  const gchar * (*get_id) (LogindSeat *object);

  gboolean  (*get_idle_hint) (LogindSeat *object);

  guint64  (*get_idle_since_hint) (LogindSeat *object);

  guint64  (*get_idle_since_hint_monotonic) (LogindSeat *object);

  GVariant * (*get_sessions) (LogindSeat *object);

};

GType logind_seat_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *logind_seat_interface_info (void);
guint logind_seat_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void logind_seat_complete_terminate (
    LogindSeat *object,
    GDBusMethodInvocation *invocation);

void logind_seat_complete_activate_session (
    LogindSeat *object,
    GDBusMethodInvocation *invocation);

void logind_seat_complete_switch_to (
    LogindSeat *object,
    GDBusMethodInvocation *invocation);

void logind_seat_complete_switch_to_next (
    LogindSeat *object,
    GDBusMethodInvocation *invocation);

void logind_seat_complete_switch_to_previous (
    LogindSeat *object,
    GDBusMethodInvocation *invocation);



/* D-Bus method calls: */
void logind_seat_call_terminate (
    LogindSeat *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean logind_seat_call_terminate_finish (
    LogindSeat *proxy,
    GAsyncResult *res,
    GError **error);

gboolean logind_seat_call_terminate_sync (
    LogindSeat *proxy,
    GCancellable *cancellable,
    GError **error);

void logind_seat_call_activate_session (
    LogindSeat *proxy,
    const gchar *arg_unnamed_arg0,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean logind_seat_call_activate_session_finish (
    LogindSeat *proxy,
    GAsyncResult *res,
    GError **error);

gboolean logind_seat_call_activate_session_sync (
    LogindSeat *proxy,
    const gchar *arg_unnamed_arg0,
    GCancellable *cancellable,
    GError **error);

void logind_seat_call_switch_to (
    LogindSeat *proxy,
    guint arg_unnamed_arg0,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean logind_seat_call_switch_to_finish (
    LogindSeat *proxy,
    GAsyncResult *res,
    GError **error);

gboolean logind_seat_call_switch_to_sync (
    LogindSeat *proxy,
    guint arg_unnamed_arg0,
    GCancellable *cancellable,
    GError **error);

void logind_seat_call_switch_to_next (
    LogindSeat *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean logind_seat_call_switch_to_next_finish (
    LogindSeat *proxy,
    GAsyncResult *res,
    GError **error);

gboolean logind_seat_call_switch_to_next_sync (
    LogindSeat *proxy,
    GCancellable *cancellable,
    GError **error);

void logind_seat_call_switch_to_previous (
    LogindSeat *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean logind_seat_call_switch_to_previous_finish (
    LogindSeat *proxy,
    GAsyncResult *res,
    GError **error);

gboolean logind_seat_call_switch_to_previous_sync (
    LogindSeat *proxy,
    GCancellable *cancellable,
    GError **error);



/* D-Bus property accessors: */
const gchar *logind_seat_get_id (LogindSeat *object);
gchar *logind_seat_dup_id (LogindSeat *object);
void logind_seat_set_id (LogindSeat *object, const gchar *value);

GVariant *logind_seat_get_active_session (LogindSeat *object);
GVariant *logind_seat_dup_active_session (LogindSeat *object);
void logind_seat_set_active_session (LogindSeat *object, GVariant *value);

gboolean logind_seat_get_can_multi_session (LogindSeat *object);
void logind_seat_set_can_multi_session (LogindSeat *object, gboolean value);

gboolean logind_seat_get_can_tty (LogindSeat *object);
void logind_seat_set_can_tty (LogindSeat *object, gboolean value);

gboolean logind_seat_get_can_graphical (LogindSeat *object);
void logind_seat_set_can_graphical (LogindSeat *object, gboolean value);

GVariant *logind_seat_get_sessions (LogindSeat *object);
GVariant *logind_seat_dup_sessions (LogindSeat *object);
void logind_seat_set_sessions (LogindSeat *object, GVariant *value);

gboolean logind_seat_get_idle_hint (LogindSeat *object);
void logind_seat_set_idle_hint (LogindSeat *object, gboolean value);

guint64 logind_seat_get_idle_since_hint (LogindSeat *object);
void logind_seat_set_idle_since_hint (LogindSeat *object, guint64 value);

guint64 logind_seat_get_idle_since_hint_monotonic (LogindSeat *object);
void logind_seat_set_idle_since_hint_monotonic (LogindSeat *object, guint64 value);


/* ---- */

#define LOGIND_TYPE_SEAT_PROXY (logind_seat_proxy_get_type ())
#define LOGIND_SEAT_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), LOGIND_TYPE_SEAT_PROXY, LogindSeatProxy))
#define LOGIND_SEAT_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), LOGIND_TYPE_SEAT_PROXY, LogindSeatProxyClass))
#define LOGIND_SEAT_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), LOGIND_TYPE_SEAT_PROXY, LogindSeatProxyClass))
#define LOGIND_IS_SEAT_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), LOGIND_TYPE_SEAT_PROXY))
#define LOGIND_IS_SEAT_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), LOGIND_TYPE_SEAT_PROXY))

typedef struct _LogindSeatProxy LogindSeatProxy;
typedef struct _LogindSeatProxyClass LogindSeatProxyClass;
typedef struct _LogindSeatProxyPrivate LogindSeatProxyPrivate;

struct _LogindSeatProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  LogindSeatProxyPrivate *priv;
};

struct _LogindSeatProxyClass
{
  GDBusProxyClass parent_class;
};

GType logind_seat_proxy_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (LogindSeatProxy, g_object_unref)
#endif

void logind_seat_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
LogindSeat *logind_seat_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
LogindSeat *logind_seat_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void logind_seat_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
LogindSeat *logind_seat_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
LogindSeat *logind_seat_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define LOGIND_TYPE_SEAT_SKELETON (logind_seat_skeleton_get_type ())
#define LOGIND_SEAT_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), LOGIND_TYPE_SEAT_SKELETON, LogindSeatSkeleton))
#define LOGIND_SEAT_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), LOGIND_TYPE_SEAT_SKELETON, LogindSeatSkeletonClass))
#define LOGIND_SEAT_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), LOGIND_TYPE_SEAT_SKELETON, LogindSeatSkeletonClass))
#define LOGIND_IS_SEAT_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), LOGIND_TYPE_SEAT_SKELETON))
#define LOGIND_IS_SEAT_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), LOGIND_TYPE_SEAT_SKELETON))

typedef struct _LogindSeatSkeleton LogindSeatSkeleton;
typedef struct _LogindSeatSkeletonClass LogindSeatSkeletonClass;
typedef struct _LogindSeatSkeletonPrivate LogindSeatSkeletonPrivate;

struct _LogindSeatSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  LogindSeatSkeletonPrivate *priv;
};

struct _LogindSeatSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType logind_seat_skeleton_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (LogindSeatSkeleton, g_object_unref)
#endif

LogindSeat *logind_seat_skeleton_new (void);


G_END_DECLS

#endif /* __LOGIND_SEAT_H__ */
