using Godot;
using System;

public class PlayerHurtSound : AudioStreamPlayer
{
    public override void _Ready()
    {
        Connect("finished", this, "_on_finished");
    }

    // public override void _Process(float delta)
    // {}

    private void _on_finished()
    {
        QueueFree();
    }
}
