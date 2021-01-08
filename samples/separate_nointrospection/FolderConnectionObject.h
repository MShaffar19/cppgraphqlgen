// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef FOLDERCONNECTIONOBJECT_H
#define FOLDERCONNECTIONOBJECT_H

#include "TodaySchema.h"

namespace graphql::today::object {

class FolderConnection
	: public service::Object
{
protected:
	explicit FolderConnection();

public:
	virtual service::FieldResult<std::shared_ptr<PageInfo>> getPageInfo(service::FieldParams&& params) const;
	virtual service::FieldResult<std::optional<std::vector<std::shared_ptr<FolderEdge>>>> getEdges(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolvePageInfo(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveEdges(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

} /* namespace graphql::today::object */

#endif // FOLDERCONNECTIONOBJECT_H
