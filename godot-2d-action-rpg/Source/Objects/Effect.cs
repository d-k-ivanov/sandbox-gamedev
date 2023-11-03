using Godot;
using System;

public partial class Effect : AnimatedSprite2D
{
    public override void _Ready()
    {
        Connect("animation_finished", new Callable(this, "_on_animation_finished"));
        Play("Animate");
    }

    public override void _Process(double delta)
    {}

    private void _on_animation_finished()
    {
        QueueFree();
    }

}
