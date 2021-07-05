import launch
import launch.actions
import launch.events

import launch_ros.actions
import launch_ros.events
import launch_ros.events.lifecycle

import lifecycle_msgs.msg


def generate_launch_description():
    """Run lifecycle nodes via launch."""
    ld = launch.LaunchDescription()

    # Prepare the talker node.
    talker_node = launch_ros.actions.LifecycleNode(
        name='talker_node',
        package='examples_rclcpp_simple_pub_sub', executable='lifecycle_talker', output='screen')

    # When the talker reaches the 'inactive' state, make it take the 'activate' transition.
    register_event_handler_for_talker_reaches_inactive_state = launch.actions.RegisterEventHandler(
        launch_ros.event_handlers.OnStateTransition(
            target_lifecycle_node=talker_node, goal_state='inactive',
            entities=[
                launch.actions.LogInfo(
                    msg="node 'talker' reached the 'inactive' state, 'activating'."),
                launch.actions.EmitEvent(event=launch_ros.events.lifecycle.ChangeState(
                    lifecycle_node_matcher=launch.events.matches_action(
                        talker_node),
                    transition_id=lifecycle_msgs.msg.Transition.TRANSITION_ACTIVATE,
                )),
            ],
        )
    )

    # When the talker node reaches the 'active' state, log a message and start and activate the listener node.
    register_event_handler_for_talker_reaches_active_state = launch.actions.RegisterEventHandler(
        launch_ros.event_handlers.OnStateTransition(
            target_lifecycle_node=talker_node, goal_state='active',
            entities=[
                launch.actions.LogInfo(
                    msg="node 'talker' reached the 'active' state, launching 'listener'."),
                launch_ros.actions.LifecycleNode(
                    name='listener',
                    package='lifecycle', executable='lifecycle_listener', output='screen'),
            ],
        )
    )

    # Make the talker node take the 'configure' transition.
    emit_event_to_request_that_talker_does_configure_transition = launch.actions.EmitEvent(
        event=launch_ros.events.lifecycle.ChangeState(
            lifecycle_node_matcher=launch.events.matches_action(
                talker_node),
            transition_id=lifecycle_msgs.msg.Transition.TRANSITION_CONFIGURE,
        )
    )

    # Add the actions to the launch description.
    # The order they are added reflects the order in which they will be executed.
    ld.add_action(register_event_handler_for_talker_reaches_inactive_state)
    ld.add_action(register_event_handler_for_talker_reaches_active_state)
    ld.add_action(talker_node)
    ld.add_action(emit_event_to_request_that_talker_does_configure_transition)

    print('Starting introspection of launch description...')
    print(launch.LaunchIntrospector().format_launch_description(ld))
    print('Starting launch of launch description...')

    return ld
