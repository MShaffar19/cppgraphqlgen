// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "TodayObjects.h"

#include "graphqlservice/introspection/Introspection.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std::literals;

namespace graphql::today {
namespace object {

Subscription::Subscription()
	: service::Object({
		"Subscription"
	}, {
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } },
		{ R"gql(nodeChange)gql"sv, [this](service::ResolverParams&& params) { return resolveNodeChange(std::move(params)); } },
		{ R"gql(nextAppointmentChange)gql"sv, [this](service::ResolverParams&& params) { return resolveNextAppointmentChange(std::move(params)); } }
	})
{
}

service::FieldResult<std::shared_ptr<Appointment>> Subscription::getNextAppointmentChange(service::FieldParams&&) const
{
	throw std::runtime_error(R"ex(Subscription::getNextAppointmentChange is not implemented)ex");
}

std::future<service::ResolverResult> Subscription::resolveNextAppointmentChange(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getNextAppointmentChange(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<Appointment>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::FieldResult<std::shared_ptr<service::Object>> Subscription::getNodeChange(service::FieldParams&&, response::IdType&&) const
{
	throw std::runtime_error(R"ex(Subscription::getNodeChange is not implemented)ex");
}

std::future<service::ResolverResult> Subscription::resolveNodeChange(service::ResolverParams&& params)
{
	auto argId = service::ModifiedArgument<response::IdType>::require("id", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getNodeChange(service::FieldParams(std::move(params), std::move(directives)), std::move(argId));
	resolverLock.unlock();

	return service::ModifiedResult<service::Object>::convert(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Subscription::resolve_typename(service::ResolverParams&& params)
{
	return service::ModifiedResult<response::StringType>::convert(response::StringType{ R"gql(Subscription)gql" }, std::move(params));
}

} /* namespace object */

void AddSubscriptionDetails(std::shared_ptr<schema::ObjectType> typeSubscription, const std::shared_ptr<schema::Schema>& schema)
{
	typeSubscription->AddFields({
		schema::Field::Make(R"gql(nextAppointmentChange)gql"sv, R"md()md"sv, std::make_optional(R"md(Need to deprecate a [field](http://spec.graphql.org/June2018/#sec-Deprecation))md"sv), schema->LookupType("Appointment")),
		schema::Field::Make(R"gql(nodeChange)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Node")), {
			schema::InputValue::Make(R"gql(id)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID")), R"gql()gql"sv)
		})
	});
}

} /* namespace graphql::today */
