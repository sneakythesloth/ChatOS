# ChatOS

**A modern, macOS-first operating environment powered by ChatGPT — with three modes: NO AI, FULL AI, and HYBRID.**

ChatOS is an ambitious desktop operating-environment project. The goal is to create the experience ourselves: a distinctive desktop, polished window and application design, thoughtful interactions, and a first-class AI collaborator. It should feel at home to macOS users while keeping a path open for Linux portability and Linux application support.

ChatOS is not intended to be a Linux distribution. macOS is the primary design and development target. Linux support is a portability objective, not the product's identity.

> **Project status:** Early-stage. The architecture, interface, operating modes, and AI interaction model are goals for development—not claims that these features are already implemented.

## The three modes

### NO AI

A complete, conventional ChatOS experience with AI functionality switched off.

- No ChatGPT assistant, AI cursor, or AI-generated actions.
- Core desktop, navigation, settings, and application workflows remain usable.
- Designed for users who want a focused traditional environment.

### FULL AI

An AI-first experience with ChatGPT integrated into the desktop.

- ChatGPT is available as a first-class system assistant.
- Ask for help with supported tasks using natural language.
- The assistant can visually demonstrate and perform supported actions through its own on-screen cursor.
- Actions should be visible, permission-aware, interruptible, and reviewable.

### HYBRID

The full conventional desktop with AI available when wanted.

- Use the desktop normally with your own cursor.
- Invoke ChatGPT for selected tasks.
- Let the assistant use its own visual cursor for an approved task, then return to normal interaction.
- AI remains optional rather than taking over the entire experience.

Users should be able to choose a mode during setup and change it later in settings.

## Product vision

### A desktop designed from the ground up

ChatOS should have its own visual identity and interaction model rather than simply reskinning an existing Linux desktop.

Design priorities:

- Modern, sleek, carefully spaced interface.
- Excellent typography, iconography, animation, and visual hierarchy.
- Fluid window management, workspaces, desktop navigation, and application launching.
- Consistent native-feeling menus, dialogs, notifications, settings, and system surfaces.
- Thoughtful keyboard shortcuts and trackpad gestures.
- Responsive performance and accessibility.
- A coherent design system so every built-in application feels like part of one product.

The visual quality bar is the polish and consistency macOS users expect, while ChatOS develops its own character.

### Two cursors: you and ChatGPT

A defining feature of ChatOS is the idea of a second, distinct cursor dedicated to the AI assistant.

- **User cursor:** Your normal pointer, controlled by your mouse or trackpad.
- **AI cursor:** A separate, clearly identifiable on-screen pointer controlled by ChatGPT when an AI task is active.

The AI cursor should make computer-use actions understandable. It can move to a control, highlight it, click, type, drag, scroll, and show what it is doing. The user should be able to distinguish assistant actions from their own input at a glance.

Important design and safety requirements:

- The AI cursor must never masquerade as the user's pointer.
- Show the assistant's current action and, where practical, its next intended step.
- Let the user pause, stop, or take control immediately.
- Ask for confirmation before consequential actions such as deleting data, sending messages, installing software, or changing security settings.
- Restrict the assistant to explicit permissions and supported application interfaces.
- Keep an auditable action history and provide clear error recovery.

**Implementation note:** macOS generally exposes one system pointer, not two independent hardware cursors. The initial implementation should therefore render a second cursor as a ChatOS-owned visual overlay and connect it to controlled UI actions or approved macOS accessibility/automation APIs. A true second OS-level pointer should not be promised unless the target platform supports it.

## Platform strategy

### macOS first

macOS is the primary platform and the initial quality target. ChatOS should integrate with macOS where appropriate—such as accessibility, permissions, windowing, input, and application launching—while creating its own desktop experience and built-in applications.

The first milestone is a polished ChatOS desktop experience running on macOS, not a replacement kernel or a Linux-based operating system.

### Linux portability

Linux is a secondary portability target. The architecture should avoid unnecessary platform lock-in so the desktop and core services can be ported to Linux over time.

Linux application support should be treated as a compatibility and packaging capability:

- Support installing and launching Linux applications when ChatOS is running on a compatible Linux host.
- Investigate standard Linux packaging formats and package managers, rather than inventing a package ecosystem without a need.
- Keep application installation separate from the desktop shell so platform-specific backends can be added.
- Clearly communicate dependencies, permissions, and compatibility to users.

Running Linux applications on macOS is a separate technical problem and should not be implied by Linux portability.

## Architecture principles

The implementation should be modular, with clear boundaries between:

1. **Desktop shell:** The desktop, panels, launcher, workspaces, window management, and system surfaces.
2. **Design system:** Shared components, typography, icons, themes, motion, and interaction patterns.
3. **Application framework:** APIs and conventions for ChatOS applications.
4. **Core services:** Settings, notifications, files, search, app lifecycle, and permissions.
5. **Platform adapters:** macOS integration first, with Linux adapters as portability develops.
6. **AI runtime:** ChatGPT connection, context management, tool orchestration, and mode policy.
7. **Computer-use controller:** The assistant cursor, visual state, input simulation, confirmations, and action history.

Prefer well-defined interfaces between modules so the AI runtime cannot bypass the same permission and safety boundaries used by the rest of the system.

## Privacy and security

- Never commit API keys, tokens, passwords, or other secrets.
- Make AI activity visible and explain what information is being shared.
- Request only the permissions needed for a task.
- Keep sensitive actions behind explicit user confirmation.
- Provide an immediate stop/pause mechanism for computer-use tasks.
- Keep user data and assistant action logs under clear user control.
- Make NO AI mode genuinely usable without an AI service connection.

## Getting started

ChatOS is at an early stage. Build, installation, and development instructions will be added once the initial macOS prototype and project structure are established.

## Roadmap

### Phase 0 — Product definition and foundations
- [ ] Define the macOS-first product scope and non-goals.
- [ ] Choose the initial implementation stack and repository structure.
- [ ] Write architecture decision records for the desktop shell, rendering, window management, and platform adapters.
- [ ] Establish coding standards, testing, CI, and release conventions.
- [ ] Create the ChatOS visual design language and interaction principles.

### Phase 1 — ChatOS desktop prototype (macOS)
- [ ] Build the desktop shell and application lifecycle.
- [ ] Create the wallpaper/desktop surface, dock or taskbar, menu/system bar, launcher, and settings.
- [ ] Implement windows, focus, resizing, snapping, and basic workspaces.
- [ ] Add polished transitions, keyboard shortcuts, and trackpad-friendly interactions.
- [ ] Build a small set of first-party applications to validate the design system.
- [ ] Make the NO AI mode complete and useful.

### Phase 2 — Design quality and daily usability
- [ ] Refine typography, icons, spacing, themes, and motion.
- [ ] Implement notifications, global search, file browsing, and app discovery.
- [ ] Add accessibility support, localization foundations, and robust error states.
- [ ] Profile startup, memory use, rendering, and interaction latency.
- [ ] Test on a range of supported macOS versions and screen sizes.

### Phase 3 — ChatGPT integration
- [ ] Design secure account/API configuration and the AI service boundary.
- [ ] Add conversational assistant UI and context controls.
- [ ] Implement the FULL AI and HYBRID mode policies.
- [ ] Define tool permissions and a user-approval model.
- [ ] Make AI availability and service failures understandable without breaking the desktop.

### Phase 4 — The second cursor and visual computer use
- [ ] Design a distinct AI cursor with clear identity, motion, and status.
- [ ] Render the cursor as a separate ChatOS visual layer.
- [ ] Build a computer-use controller for supported ChatOS-native interfaces.
- [ ] Add visual targeting, click, typing, scrolling, and drag actions.
- [ ] Add live action narration/status, pause, stop, and user takeover.
- [ ] Add confirmation gates for high-impact actions.
- [ ] Record action history and support recovery from failed tasks.
- [ ] Explore macOS Accessibility APIs and approved automation interfaces for external apps, with explicit user permissions.
- [ ] Test reliability and prevent unintended actions.

### Phase 5 — Application ecosystem
- [ ] Define a ChatOS application API and developer documentation.
- [ ] Build first-party applications that demonstrate consistent system integration.
- [ ] Create an app discovery and installation experience.
- [ ] Design app sandboxing, permissions, updates, and uninstall flows.
- [ ] Support macOS applications through the appropriate macOS mechanisms.

### Phase 6 — Linux portability
- [ ] Separate platform-independent desktop/core code from macOS-specific services.
- [ ] Implement a Linux platform adapter.
- [ ] Bring up the desktop shell on a selected Linux environment.
- [ ] Add Linux application installation and launching through established package formats/managers.
- [ ] Document platform differences and test supported distributions.
- [ ] Keep macOS as the primary product and design reference.

### Phase 7 — Beta and release readiness
- [ ] Complete end-to-end tests for NO AI, FULL AI, and HYBRID.
- [ ] Conduct security, privacy, accessibility, and performance reviews.
- [ ] Add crash reporting with appropriate privacy controls and user consent.
- [ ] Publish installation, upgrade, backup, and recovery documentation.
- [ ] Establish a release channel and issue-reporting process.

## Contributing

Ideas, design feedback, bug reports, and contributions are welcome. When opening an issue or pull request, describe the affected platform, operating mode, and relevant testing.

## License

No license has been specified yet. Until a license is added to this repository, do not assume that the code is available for reuse, modification, or redistribution.
