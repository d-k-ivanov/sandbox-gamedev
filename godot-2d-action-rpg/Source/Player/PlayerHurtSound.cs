using Godot;
using System;

public partial class PlayerHurtSound : AudioStreamPlayer
{
    public override void _Ready()
    {
        Connect("finished", new Callable(this, "_on_finished"));
    }

    // public override void _Process(float delta)
    // {}

    private void _on_finished()
    {
        QueueFree();
    }
}
