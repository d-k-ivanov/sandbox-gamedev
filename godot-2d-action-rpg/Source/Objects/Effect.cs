using Godot;
using System;

public class Effect : AnimatedSprite
{
    public override void _Ready()
    {
        Connect("animation_finished", this, "_on_animation_finished");
        Play("Animate");
    }

    public override void _Process(float delta)
    {}

    private void _on_animation_finished()
    {
        QueueFree();
    }

}
