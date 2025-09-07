#pragma once

namespace FW {
    class Serializable {
    public:
        virtual ~Serializable() = default;

        virtual void serialize() = 0;
        virtual void deserialize() = 0;
    };
}