/*
 * Copyright (C) Research In Motion Limited 2010. All rights reserved.
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
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_SVG_SVG_RESOURCES_CYCLE_SOLVER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_SVG_SVG_RESOURCES_CYCLE_SOLVER_H_

#include "third_party/blink/renderer/core/layout/svg/layout_svg_resource_container.h"
#include "third_party/blink/renderer/platform/wtf/allocator/allocator.h"
#include "third_party/blink/renderer/platform/wtf/hash_set.h"

namespace blink {

// This class traverses the graph formed by SVGResources of
// LayoutObjects, maintaining the active path as LayoutObjects are
// visited. It also maintains a cache of sub-graphs that has already
// been visited and that does not contain any cycles.
class SVGResourcesCycleSolver {
  STACK_ALLOCATED();

 public:
  SVGResourcesCycleSolver();
  SVGResourcesCycleSolver(const SVGResourcesCycleSolver&) = delete;
  SVGResourcesCycleSolver& operator=(const SVGResourcesCycleSolver&) = delete;
  ~SVGResourcesCycleSolver();

  bool IsKnownAcyclic(const LayoutSVGResourceContainer*) const;
  void AddAcyclicSubgraph(const LayoutSVGResourceContainer*);

  class Scope {
    STACK_ALLOCATED();

   public:
    Scope(SVGResourcesCycleSolver& solver)
        : solver_(solver), resource_(nullptr) {}
    ~Scope() {
      if (resource_)
        solver_.LeaveResource(resource_);
    }

    bool Enter(const LayoutSVGResourceContainer* resource) {
      if (!solver_.EnterResource(resource))
        return false;
      resource_ = resource;
      return true;
    }

   private:
    SVGResourcesCycleSolver& solver_;
    const LayoutSVGResourceContainer* resource_;
  };

 private:
  bool EnterResource(const LayoutSVGResourceContainer*);
  void LeaveResource(const LayoutSVGResourceContainer*);

  using ResourceSet = HashSet<const LayoutSVGResourceContainer*>;
  ResourceSet active_resources_;
  ResourceSet dag_cache_;
};

}  // namespace blink

#endif
