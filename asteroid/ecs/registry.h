#pragma once
#include <iostream>
#include <array>
#include <unordered_map>
#include <stdexcept>
#include <bitset>
#include <string>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "component.h"
#include <stack>

constexpr size_t MAX_ENTITIES = 128;

using Entity = int;

// Registry class
class Registry {
public:
    Registry() = default;

    Entity createEntity() {
        if (nextEntity >= MAX_ENTITIES) 
        {
             if (!freeIndex.empty())
            { 
                int next = freeIndex.top();
                freeIndex.pop();
                return next;
            }
            else 
            {
                throw std::runtime_error("No available entities");
            }
        }else
            return nextEntity++;
    }

    template<Component T>
    void setComponent(Entity entity, T& component) {
        static_assert(is_component_v<T>, "Invalid component type");
        component.entityID = entity;
        auto componentType = getComponentType<T>();
        componentStorage<T>()[entity] = component;
        componentMasks[entity].set(static_cast<size_t>(componentType));
    }

    template<typename T>
    std::array<T, MAX_ENTITIES>& getAll() {
        static_assert(is_component_v<T>, "Invalid component type");
        return componentStorage<T>();
    }

    Entity getEntity(int entityID) const {
        return entityID;
    }

    template<typename T>
    T& getComponent(Entity entity) {
        static_assert(is_component_v<T>, "Invalid component type");
        return componentStorage<T>()[entity];
    }

    bool hasComponent(Entity entity, ComponentType componentType) const {
        return componentMasks[entity].test(static_cast<size_t>(componentType));
    }

    void remove(Entity entity) {
        for (size_t i = 0; i < static_cast<size_t>(ComponentType::LAST); ++i) {
            componentStorage<PlayerComp>()[entity] = {};
            componentStorage<AsteroidComp>()[entity] = {};
            componentStorage<BulletComp>()[entity] = {};
            componentStorage<TransformComp>()[entity] = {};
        }

        freeIndex.push(entity);
        componentMasks[entity].reset();
    }

    template<typename T>
    size_t countEntitiesWithComponent()  {
        static_assert(is_component_v<T>, "Invalid component type");

        const auto& components = componentStorage<T>();
        size_t count = 0;

        for (const auto& component : components) {
            if (componentIsValid(component))
            {
                count++;
            }
        }

        return count;
    }

    template<Component T>
    inline bool componentIsValid(const T& component) const {
        return component.entityID != -1;
    }

private:
    std::stack<int> freeIndex;
    template<typename T>
    using ComponentArray = std::array<T, MAX_ENTITIES>;

    ComponentArray<TransformComp> transforms{};
    ComponentArray<PlayerComp> players{};
    ComponentArray<AsteroidComp> asteroids{};
    ComponentArray<BulletComp> bullets{};
    ComponentArray<RenderOptionComp> renderOptions{};

    std::array<std::bitset<static_cast<size_t>(ComponentType::LAST)>, MAX_ENTITIES> componentMasks{};
    Entity nextEntity = 0;

    template<typename T>
    static constexpr bool is_component_v = std::disjunction_v<
        std::is_same<T, PlayerComp>,
        std::is_same<T, AsteroidComp>,
        std::is_same<T, BulletComp>,
        std::is_same<T, TransformComp>,
        std::is_same<T, RenderOptionComp>
    >;

    template<typename T>
    ComponentType getComponentType() const {
        if constexpr (std::is_same_v<T, PlayerComp>) {
            return ComponentType::Player;
        }
        else if constexpr (std::is_same_v<T, AsteroidComp>) {
            return ComponentType::Asteroid;
        }
        else if constexpr (std::is_same_v<T, BulletComp>) {
            return ComponentType::Bullet;
        }
        else if constexpr (std::is_same_v<T, TransformComp>) {
            return ComponentType::Transform;
        }
        else if constexpr (std::is_same_v<T, RenderOptionComp>) {
            return ComponentType::RenderOption;
        }
    }

    template<typename T>
    ComponentArray<T>& componentStorage() {
        if constexpr (std::is_same_v<T, PlayerComp>) {
            return players;
        }
        else if constexpr (std::is_same_v<T, AsteroidComp>) {
            return asteroids;
        }
        else if constexpr (std::is_same_v<T, BulletComp>) {
            return bullets;
        }
        else if constexpr (std::is_same_v<T, TransformComp>) {
            return transforms;
        }
        else if constexpr (std::is_same_v<T, RenderOptionComp>) {
            return renderOptions;
        }
    }
};
