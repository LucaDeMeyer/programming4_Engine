#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

namespace dae
{
    class GameObject;
    class BaseComponent
    {
    public:
        virtual ~BaseComponent() = default;

        BaseComponent(const BaseComponent& other) = delete;
        BaseComponent(BaseComponent&& other) = delete;
        BaseComponent& operator=(BaseComponent&& other) = delete;
        BaseComponent& operator=(const BaseComponent& other) = delete;

        virtual void Update() = 0;
        virtual void Render() const = 0;        // not too sure about having these abstract functions in here
    											//-> not every comp needs to update or render, fine for now 
        virtual void RenderUI() {}

        dae::GameObject* GetOwner() const { return m_pOwner; }

        void MarkForDestruction()
        {
            m_isMarkedForDestruction = true;
        }

        bool IsMarkedForDestruction() const
        {
            return m_isMarkedForDestruction;
        }

    protected:
        explicit BaseComponent(dae::GameObject* m_pOwner) : m_pOwner(m_pOwner) {}
        dae::GameObject* m_pOwner{ nullptr };

    private:
        bool m_isMarkedForDestruction = false;
    };
};
#endif