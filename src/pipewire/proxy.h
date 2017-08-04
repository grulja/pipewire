/* PipeWire
 * Copyright (C) 2015 Wim Taymans <wim.taymans@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __PIPEWIRE_PROXY_H__
#define __PIPEWIRE_PROXY_H__

#ifdef __cplusplus
extern "C" {
#endif

struct pw_remote;

/** \page page_proxy Proxy
 *
 * \section sec_page_proxy_overview Overview
 *
 * The proxy object is a client side representation of a resource
 * that lives on the server.
 *
 * It is used to communicate with the server side object.
 *
 * \section sec_page_proxy_create Create
 *
 * A client first creates a new proxy object with pw_proxy_new(). A
 * type must be provided for this object.
 *
 * The protocol of the context will usually install an interface to
 * translate method calls and events to the wire format.
 *
 * The creator of the proxy will usually also install an event
 * implementation of the particular object type.
 *
 * \section sec_page_proxy_bind Bind
 *
 * To actually use the proxy object, one needs to create a server
 * side resource for it. This can be done by, for example, binding
 * to a global object or by calling a method that creates and binds
 * to a new remote object. In all cases, the local id is passed to
 * the server and is used to create a resource with the same id.
 *
 * \section sec_page_proxy_methods Methods
 *
 * To call a method on the proxy use the interface methods. Calling
 * any interface method will result in a request to the server to
 * perform the requested action on the corresponding resource.
 *
 * \section sec_page_proxy_events Events
 *
 * Events send from the server to the proxy will be demarshalled by
 * the protocol and will then result in a call to the installed
 * implementation of the proxy.
 *
 * \section sec_page_proxy_destroy Destroy
 *
 * Use pw_proxy_destroy() to destroy the client side object. This
 * is usually done automatically when the server removes the resource
 * associated to the proxy.
 */

/** \class pw_proxy
 *
 * \brief Represents an object on the client side.
 *
 * A pw_proxy acts as a client side proxy to an object existing in the
 * pipewire server. The proxy is responsible for converting interface functions
 * invoked by the client to PipeWire messages. Events will call the handlers
 * set in listener.
 *
 * See \ref page_proxy
 */
struct pw_proxy;

#include <pipewire/type.h>
#include <pipewire/utils.h>
#include <pipewire/callback.h>

struct pw_proxy_callbacks {
#define PW_VERSION_PROXY_CALLBACKS   0
        uint32_t version;

        void (*destroy) (void *data);
};

/** Make a new proxy object. The id can be used to bind to a remote object. */
struct pw_proxy *
pw_proxy_new(struct pw_proxy *proxy,	/**< proxy as factory */
	     uint32_t type,		/**< interface type */
	     size_t user_data_size	/**< size of user data */);

void
pw_proxy_add_callbacks(struct pw_proxy *proxy,
		       struct pw_callback_info *info,
		       const struct pw_proxy_callbacks *callbacks,
		       void *data);

void
pw_proxy_add_listener(struct pw_proxy *proxy,	/**< the proxy */
		      struct pw_callback_info *info,
		      const void *callbacks,	/**< events */
		      void *data		/**< data passed to events */);

void pw_proxy_destroy(struct pw_proxy *proxy);

void *pw_proxy_get_user_data(struct pw_proxy *proxy);

uint32_t pw_proxy_get_id(struct pw_proxy *proxy);

struct pw_callback_list *pw_proxy_get_listeners(struct pw_proxy *proxy);

const void *pw_proxy_get_implementation(struct pw_proxy *proxy);

#define pw_proxy_notify(p,type,event,...)	pw_callback_emit(pw_proxy_get_listeners(p),type,event,__VA_ARGS__)
#define pw_proxy_notify_na(p,type,event)	pw_callback_emit_na(pw_proxy_get_listeners(p),type,event)

#define pw_proxy_do(p,type,method,...)		((type*) pw_proxy_get_implementation(p))->method(p, __VA_ARGS__)
#define pw_proxy_do_na(p,type,method)		((type*) pw_proxy_get_implementation(p))->method(p)

#ifdef __cplusplus
}
#endif

#endif /* __PIPEWIRE_PROXY_H__ */
